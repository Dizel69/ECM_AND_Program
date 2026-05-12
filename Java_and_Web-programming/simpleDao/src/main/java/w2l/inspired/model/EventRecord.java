package w2l.inspired.model;

import jakarta.persistence.Column;
import jakarta.persistence.Entity;
import jakarta.persistence.GeneratedValue;
import jakarta.persistence.GenerationType;
import jakarta.persistence.Id;
import jakarta.persistence.PrePersist;
import jakarta.persistence.Table;

import java.time.LocalDateTime;

@Entity
@Table(name = "event_records")
public class EventRecord {

    @Id
    @GeneratedValue(strategy = GenerationType.IDENTITY)
    private Long id;

    @Column(nullable = false, length = 2000)
    private String description;

    @Column(nullable = false)
    private int points;

    @Column(nullable = false)
    private LocalDateTime createdAt;

    protected EventRecord() {
    }

    public EventRecord(String description, int points) {
        this.description = description;
        this.points = points;
    }

    @PrePersist
    void prePersist() {
        if (createdAt == null) {
            createdAt = LocalDateTime.now();
        }
    }

    public Long getId() {
        return id;
    }

    public String getDescription() {
        return description;
    }

    public int getPoints() {
        return points;
    }

    public LocalDateTime getCreatedAt() {
        return createdAt;
    }
}
