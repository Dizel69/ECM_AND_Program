package w2l.inspired.web;

import org.springframework.stereotype.Controller;
import org.springframework.ui.Model;
import org.springframework.web.bind.annotation.GetMapping;
import org.springframework.web.bind.annotation.PostMapping;
import org.springframework.web.bind.annotation.RequestParam;
import org.springframework.web.servlet.mvc.support.RedirectAttributes;
import w2l.inspired.service.ScoreNotebookService;

@Controller
public class NotebookController {

    private final ScoreNotebookService notebookService;

    public NotebookController(ScoreNotebookService notebookService) {
        this.notebookService = notebookService;
    }

    @GetMapping("/")
    public String index(Model model) {
        int totalScore = notebookService.totalScore();
        model.addAttribute("pendingTasks", notebookService.listPendingRecentFirst());
        model.addAttribute("records", notebookService.listRecentFirst());
        model.addAttribute("totalScore", totalScore);
        model.addAttribute("scoreClass", resolveScoreClass(totalScore));
        return "notebook";
    }

    @PostMapping("/tasks/create")
    public String createTask(@RequestParam String description, @RequestParam int magnitude) {
        notebookService.createPendingTask(description, magnitude);
        return "redirect:/";
    }

    @PostMapping("/tasks/complete")
    public String completeTask(@RequestParam long taskId, @RequestParam String outcome) {
        notebookService.completePendingTask(taskId, outcome);
        return "redirect:/";
    }

    /** Старая форма с кэшированной страницы — иначе браузер бил в несуществующий URL и получал 404. */
    @PostMapping("/events")
    public String legacyJournalForm(RedirectAttributes redirectAttributes) {
        redirectAttributes.addFlashAttribute(
                "error",
                "Страница устарела — нажмите обновить (F5). Запись в журнал теперь: сначала «Новая задача», потом кнопки в списке."
        );
        return "redirect:/";
    }

    @PostMapping("/events/delete")
    public String delete(@RequestParam long id) {
        notebookService.deleteById(id);
        return "redirect:/";
    }

    private static String resolveScoreClass(int totalScore) {
        if (totalScore < 0) {
            return "score-neg";
        }
        if (totalScore < 100) {
            return "score-warn";
        }
        return "score-good";
    }
}
