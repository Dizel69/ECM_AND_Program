"""Задание 2.2. Транскрибация аудио, в котором несколько языков.

Как запустить, из папки 2.2:

    source .venv/bin/activate
    python transcribe.py

В терминал печатаются фрагменты: время, язык и текст. Окна нет.

Это развитие программы 2024 года (Whisper GUI). Там язык выбирался один
на весь файл, например только русский. Так делается базовое задание 2.1.
Здесь язык не задаётся. Модель смотрит каждый кусок записи отдельно
и пишет речь как услышала: русский остаётся русским, английский английским.
В перевод это не превращается.

Почему не один вызов transcribe на весь файл. У faster-whisper параметр
multilingual как раз обещает определять язык заново. На деле библиотека
сначала вырезает тишину и склеивает речь в окна примерно по 30 секунд.
Язык выбирается один на такое окно. Если в первые секунды окна русский,
английская фраза следом пропадает или переписывается по-русски.
Поэтому паузы ищем сами и каждый такой кусок отдаём модели со своим языком.

Интернет после первой загрузки не нужен. Веса лежат в папке models/.
Модель своя не обучалась. Это готовый Whisper в формате faster-whisper:
та же сеть OpenAI, только упакованная для запуска на процессоре.
"""

from pathlib import Path

import numpy as np
from faster_whisper import WhisperModel
from faster_whisper.audio import decode_audio
from faster_whisper.vad import VadOptions, get_speech_timestamps

# Какой файл слушать. Меняется только эта строка.
# Путь относительно папки 2.2. Можно подставить свой mp3, wav, m4a, ogg, flac.
#AUDIO = "audio/several_languages.wav"
AUDIO = "audio/Гидромет.m4a"

# Размер сети. Все варианты многоязычные, кроме имён с суффиксом .en.
#   tiny   очень быстро, на русском часто ошибается
#   base   быстрее, для короткой проверки
#   small  то, что стоит сейчас: нормальная точность на процессоре
#   medium ещё точнее и заметно медленнее
MODEL_SIZE = "small"

# Пауза, после которой кусок считается новой фразой и язык смотрят заново.
# Между разными языками она должна быть. Короткие запинки внутри фразы
# (меньше этого порога) остаются в том же куске.
PAUSE_SECONDS = 0.6

# Короткие имена языков, которые чаще всего встретятся на защите.
# Если кода нет в списке, в терминал выводится сам код, например "pl".
LANGUAGE_NAMES = {
    "ru": "русский",
    "en": "английский",
    "de": "немецкий",
    "fr": "французский",
    "es": "испанский",
    "uk": "украинский",
    "it": "итальянский",
    "zh": "китайский",
}

# Папка этого файла, а не та, откуда его запустили.
ROOT = Path(__file__).resolve().parent

# Сюда faster-whisper кладёт скачанные веса. Повторный запуск сеть не трогает.
MODELS = ROOT / "models"

# Whisper всегда слушает 16000 отсчётов в секунду, один канал.
RATE = 16000


def main() -> None:
    path = Path(AUDIO)
    if not path.is_absolute():
        path = ROOT / path
    if not path.is_file():
        raise SystemExit(f"Файл не найден: {path}")

    # device="cpu" — видеокарты нет, считаем на процессоре.
    # compute_type="int8" — веса хранятся компактнее, скорость выше,
    # качество для этого задания почти не проседает.
    # download_root — локальная папка models/, не системный кэш.
    model = WhisperModel(
        MODEL_SIZE,
        device="cpu",
        compute_type="int8",
        download_root=str(MODELS),
    )

    # На выходе float32 от -1 до 1. Так же звук ждёт detect_language.
    waveform = decode_audio(str(path), sampling_rate=RATE)

    print(f"Файл: {path.name}")
    print(f"Длительность: {len(waveform) / RATE:.1f} с")
    print(f"Модель: {MODEL_SIZE}, локально, без перевода на один язык")
    print()

    found = False
    for start, end in speech_spans(waveform):
        piece = waveform[start:end]
        language, probability = language_of(model, piece)
        # language= найденный код, а не None и не один язык на весь файл.
        # task="transcribe" пишет услышанное. "translate" перевёл бы на английский.
        # vad_filter выключен: тишину мы уже вырезали, второй раз она не нужна.
        # condition_on_previous_text=False: предыдущая фраза не тянет следующую.
        segments, _info = model.transcribe(
            piece,
            language=language,
            task="transcribe",
            vad_filter=False,
            condition_on_previous_text=False,
        )
        lines = [segment.text.strip() for segment in segments if segment.text.strip()]
        if not lines:
            continue
        found = True
        name = LANGUAGE_NAMES.get(language, language)
        print(
            f"[{start / RATE:05.1f}-{end / RATE:05.1f}] "
            f"{name} ({probability:.0%})"
        )
        for line in lines:
            print(line)
        print()

    if not found:
        print("Речь не найдена.")


def speech_spans(waveform: np.ndarray) -> list[tuple[int, int]]:
    """Границы фраз в отсчётах.

    Silero VAD из состава библиотеки отмечает, где есть голос.
    Соседние отметки склеиваются, если между ними пауза короче
    PAUSE_SECONDS. Так одна фраза не рвётся на каждое «э-э»,
    а смена языка после паузы становится отдельным куском.
    """
    chunks = get_speech_timestamps(
        waveform,
        VadOptions(min_silence_duration_ms=400),
    )
    if not chunks:
        return []

    spans: list[tuple[int, int]] = []
    start = chunks[0]["start"]
    end = chunks[0]["end"]
    for chunk in chunks[1:]:
        gap = (chunk["start"] - end) / RATE
        if gap < PAUSE_SECONDS:
            end = chunk["end"]
        else:
            spans.append((start, end))
            start = chunk["start"]
            end = chunk["end"]
    spans.append((start, end))
    return spans


def language_of(model: WhisperModel, piece: np.ndarray) -> tuple[str, float]:
    """Язык одного куска. На вход звук 16000 Гц, один канал, float.

    Совсем короткий обрывок детектору не из чего слушать,
    поэтому тишиной добиваем до секунды. На распознавание это не влияет:
    в transcribe уходит исходный кусок, без этой добивки.
    """
    sample = piece
    if sample.size < RATE:
        sample = np.pad(sample, (0, RATE - sample.size))
    language, probability, _variants = model.detect_language(sample)
    return language, probability


if __name__ == "__main__":
    main()
