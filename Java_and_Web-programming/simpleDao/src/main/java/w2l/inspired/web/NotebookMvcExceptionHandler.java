package w2l.inspired.web;

import org.springframework.transaction.TransactionSystemException;
import org.springframework.web.bind.MethodArgumentNotValidException;
import org.springframework.web.bind.annotation.ControllerAdvice;
import org.springframework.web.bind.annotation.ExceptionHandler;
import org.springframework.web.method.annotation.MethodArgumentTypeMismatchException;
import org.springframework.web.servlet.mvc.support.RedirectAttributes;

/**
 * Ошибки валидации и биндинга формы уводим на главную со вспышкой — без Whitelabel.
 */
@ControllerAdvice
public class NotebookMvcExceptionHandler {

    private static IllegalArgumentException findIllegalArgumentInChain(Throwable ex) {
        Throwable t = ex;
        while (t != null) {
            if (t instanceof IllegalArgumentException iae) {
                return iae;
            }
            Throwable next = t.getCause();
            if (next == null || next == t) {
                break;
            }
            t = next;
        }
        return null;
    }

    @ExceptionHandler(IllegalArgumentException.class)
    public String handleIllegalArgument(IllegalArgumentException ex, RedirectAttributes redirectAttributes) {
        redirectAttributes.addFlashAttribute("error", ex.getMessage());
        return "redirect:/";
    }

    @ExceptionHandler(TransactionSystemException.class)
    public String handleTransactional(TransactionSystemException ex, RedirectAttributes redirectAttributes) {
        IllegalArgumentException ia = findIllegalArgumentInChain(ex);
        if (ia != null) {
            redirectAttributes.addFlashAttribute("error", ia.getMessage());
            return "redirect:/";
        }
        throw ex;
    }

    @ExceptionHandler(MethodArgumentTypeMismatchException.class)
    public String handleTypeMismatch(MethodArgumentTypeMismatchException ex, RedirectAttributes redirectAttributes) {
        redirectAttributes.addFlashAttribute("error", "Неверный формат поля формы («" + ex.getName() + "»). Введите целое число.");
        return "redirect:/";
    }

    @ExceptionHandler(MethodArgumentNotValidException.class)
    public String handleValidation(MethodArgumentNotValidException ex, RedirectAttributes redirectAttributes) {
        redirectAttributes.addFlashAttribute("error", "Проверьте заполнение полей.");
        return "redirect:/";
    }
}
