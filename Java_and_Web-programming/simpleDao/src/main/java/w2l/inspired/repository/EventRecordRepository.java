package w2l.inspired.repository;

import org.springframework.data.jpa.repository.JpaRepository;
import org.springframework.data.jpa.repository.Query;
import w2l.inspired.model.EventRecord;

import java.util.List;

public interface EventRecordRepository extends JpaRepository<EventRecord, Long> {

    List<EventRecord> findAllByOrderByCreatedAtDesc();

    @Query("SELECT SUM(e.points) FROM EventRecord e")
    Long sumPointsTotal();
}
