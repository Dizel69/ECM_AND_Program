package w2l.inspired;

public final class NotebookConstants {

    private NotebookConstants() {
    }

    public static final String EXAMPLE_DESCRIPTION = "Например: сходил на пары";

    /** Допустимое значение балла в журнале (после «Готово» / «Не готово»). */
    public static final int POINTS_MIN = -1_000_000;
    public static final int POINTS_MAX = 1_000_000;

    /** Положительная величина при создании задачи (до выбора исхода). */
    public static final int TASK_MAGNITUDE_MIN = 1;
    public static final int TASK_MAGNITUDE_MAX = 1_000_000;
}
