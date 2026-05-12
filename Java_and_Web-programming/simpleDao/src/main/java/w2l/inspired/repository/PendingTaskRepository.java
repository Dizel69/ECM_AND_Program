package w2l.inspired.repository;

import org.springframework.data.jpa.repository.JpaRepository;
import w2l.inspired.model.PendingTask;

import java.util.List;

public interface PendingTaskRepository extends JpaRepository<PendingTask, Long> {

    List<PendingTask> findAllByOrderByCreatedAtDesc();
}
