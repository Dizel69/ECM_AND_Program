from .base_page import BasePage
from utils.config import Config

class LoginPage(BasePage):
    URL = f"{Config.BASE_URL}/login"
    USERNAME_INPUT = "#username"
    PASSWORD_INPUT = "#password"
    LOGIN_BUTTON = "button[type='submit']"
    ERROR_MESSAGE = ".error-alert"

    def load(self):
        self.navigate_to(self.URL)

    def login(self, username: str, password: str):
        self.fill(self.USERNAME_INPUT, username)
        self.fill(self.PASSWORD_INPUT, password)
        self.click(self.LOGIN_BUTTON)

    def get_error_message(self) -> str:
        return self.get_text(self.ERROR_MESSAGE)