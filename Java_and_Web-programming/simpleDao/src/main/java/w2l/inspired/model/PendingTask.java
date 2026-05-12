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
@Table(name = "pending_tasks")
public class PendingTask {

    @Id
    @GeneratedValue(strategy = GenerationType.IDENTITY)
    private Long id;

    @Column(nullable = false, length = 2000)
    private String description;

    /**
     * Величина начисления/списания (положительное число). Знак задаётся при «Готово» / «Не готово».
     */
    @Column(nullable = false)
    private int magnitude;

    @Column(nullable = false)
    private LocalDateTime createdAt;

    protected PendingTask() {
    }

    public PendingTask(String description, int magnitude) {
        this.description = description;
        this.magnitude = magnitude;
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

    public int getMagnitude() {
        return magnitude;
    }

    public LocalDateTime getCreatedAt() {
        return createdAt;
    }
}
