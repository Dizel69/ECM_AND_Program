import time

from playwright.sync_api import Page, expect
from utils.config import Config

class BasePage:
    def __init__(self, page: Page):
        self.page = page
        self.base_locator = ""

    def open(self, URL):
        self.page.goto(URL)

    def wait_page(self):
        start_time = time.time()
        while(True):
            current_time = time.time()
            timeout = float(Config.ELEMEMT_SEARCH_TIMEOUT_S)
            if current_time - start_time > timeout:
                raise TimeoutException("ELEMEMT_SEARCH_TIMEOUT_S is over")
            if self.page.is_visible:
                return


    def wait_for_element(self, selector: str, timeout: int = 10000):
        self.page.wait_for_selector(selector, timeout=timeout)

    def click(self, selector: str):
        self.page.click(selector)

    def fill(self, selector: str, value: str):
        self.page.fill(selector, value)

    def get_text(self, selector: str) -> str:
        return self.page.text_content(selector)

    def is_visible(self, selector: str) -> bool:
        return self.page.is_visible(selector)

    def assert_text(self, selector: str, expected_text: str):
        expect(self.page.locator(selector)).to_have_text(expected_text)

    def assert_url(self, expected_url: str):
        expect(self.page).to_have_url(expected_url)