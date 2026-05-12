-- Выполняется после Hibernate (defer-datasource): подписи в метаданных и «удобочитаемые» представления для Adminer / pgAdmin.
-- Если таблиц ещё нет — пропускаем (первый старт возможен без данных).

COMMENT ON TABLE event_records IS 'Журнал приложения «Блокнот баллов»: каждая строка — закрытая задача («Готово» = начисление, «Не готово» = списание).';
COMMENT ON COLUMN event_records.id IS 'Уникальный номер записи (автоинкремент).';
COMMENT ON COLUMN event_records.description IS 'Текст задачи в журнал.';
COMMENT ON COLUMN event_records.points IS 'Изменение счёта: положительно — выполнено, отрицательно — не выполнено.';
COMMENT ON COLUMN event_records.created_at IS 'Момент, когда задача перенесена в журнал.';

COMMENT ON TABLE pending_tasks IS 'Активные задачи до нажатия «Готово» или «Не готово».';
COMMENT ON COLUMN pending_tasks.id IS 'Номер задачи.';
COMMENT ON COLUMN pending_tasks.description IS 'Что нужно сделать.';
COMMENT ON COLUMN pending_tasks.magnitude IS 'Число N: в журнал уйдёт +N или −N.';
COMMENT ON COLUMN pending_tasks.created_at IS 'Когда задача попала в список активных.';

CREATE OR REPLACE VIEW v_journal_for_ui AS
SELECT
  event_records.id AS "№",
  description AS "Описание",
  CASE
    WHEN points >= 0 THEN '+' || CAST(points AS TEXT)
    ELSE CAST(points AS TEXT)
  END AS "Баллы (текст)",
  points AS "Баллы (число)",
  created_at AS "Записано"
FROM event_records;

CREATE OR REPLACE VIEW v_tasks_queue_for_ui AS
SELECT
  pending_tasks.id AS "№",
  description AS "Задача",
  magnitude AS "N (±)",
  '+' || CAST(magnitude AS TEXT) AS "При исходе Готово",
  '-' || CAST(magnitude AS TEXT) AS "При исходе Не готово",
  created_at AS "Добавлено"
FROM pending_tasks;

COMMENT ON VIEW v_journal_for_ui IS 'Удобный просмотр журнала: подписанные столбцы. Использовать вместо голой таблицы event_records в Adminer.';
COMMENT ON VIEW v_tasks_queue_for_ui IS 'Удобный просмотр очереди задач перед закрытием. Аналог pending_tasks с подписанными столбцами.';
