package w2l.inspired.service;

import org.springframework.stereotype.Service;
import org.springframework.transaction.annotation.Transactional;
import w2l.inspired.NotebookConstants;
import w2l.inspired.model.EventRecord;
import w2l.inspired.model.PendingTask;
import w2l.inspired.repository.EventRecordRepository;
import w2l.inspired.repository.PendingTaskRepository;

import java.util.List;

@Service
public class ScoreNotebookService {

    private final EventRecordRepository eventRecordRepository;
    private final PendingTaskRepository pendingTaskRepository;

    public ScoreNotebookService(EventRecordRepository eventRecordRepository,
                                PendingTaskRepository pendingTaskRepository) {
        this.eventRecordRepository = eventRecordRepository;
        this.pendingTaskRepository = pendingTaskRepository;
    }

    public List<PendingTask> listPendingRecentFirst() {
        return pendingTaskRepository.findAllByOrderByCreatedAtDesc();
    }

    public List<EventRecord> listRecentFirst() {
        return eventRecordRepository.findAllByOrderByCreatedAtDesc();
    }

    public int totalScore() {
        Long sum = eventRecordRepository.sumPointsTotal();
        return sum != null ? sum.intValue() : 0;
    }

    @Transactional
    public PendingTask createPendingTask(String description, int magnitude) {
        String trimmed = description == null ? "" : description.trim();
        if (trimmed.isEmpty()) {
            throw new IllegalArgumentException("Описание не может быть пустым");
        }
        int m = Math.abs(magnitude);
        if (m < NotebookConstants.TASK_MAGNITUDE_MIN || m > NotebookConstants.TASK_MAGNITUDE_MAX) {
            throw new IllegalArgumentException(
                    "Величина баллов для задачи: от "
                            + NotebookConstants.TASK_MAGNITUDE_MIN
                            + " до "
                            + NotebookConstants.TASK_MAGNITUDE_MAX
            );
        }
        return pendingTaskRepository.save(new PendingTask(trimmed, m));
    }

    @Transactional
    public void completePendingTask(long taskId, String outcome) {
        PendingTask task = pendingTaskRepository.findById(taskId).orElseThrow(
                () -> new IllegalArgumentException("Задача не найдена (возможно, уже выполнена).")
        );
        int magnitude = Math.abs(task.getMagnitude());
        int signedPoints = "miss".equalsIgnoreCase(outcome) ? -magnitude : magnitude;
        persistJournalEntry(task.getDescription(), signedPoints);
        pendingTaskRepository.delete(task);
    }

    @Transactional
    public void deleteById(long id) {
        eventRecordRepository.deleteById(id);
    }

    private void persistJournalEntry(String description, int signedPoints) {
        String trimmed = description == null ? "" : description.trim();
        if (trimmed.isEmpty()) {
            throw new IllegalArgumentException("Описание не может быть пустым");
        }
        if (signedPoints < NotebookConstants.POINTS_MIN || signedPoints > NotebookConstants.POINTS_MAX) {
            throw new IllegalArgumentException(
                    "Баллы в журнале должны быть от "
                            + NotebookConstants.POINTS_MIN
                            + " до "
                            + NotebookConstants.POINTS_MAX
            );
        }
        eventRecordRepository.save(new EventRecord(trimmed, signedPoints));
    }
}
