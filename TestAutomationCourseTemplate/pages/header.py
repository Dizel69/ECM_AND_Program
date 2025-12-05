import time
import re
from playwright.sync_api import expect
from playwright.sync_api import Page, expect
from pages.base_page import BasePage
from utils.config import Config

class PMKPage(BasePage):

    def __init__(self, page: Page):
        super().__init__(page)
        self.wait_page()

    def click_link_by_text(self, link_text: str):
        """
        Кликает по любой ссылке <a> с указанным текстом
        """
        selector = f'//a[text()="{link_text}"]'
        self.page.wait_for_selector(selector, timeout=Config.ELEMEMT_SEARCH_TIMEOUT_S)
        self.page.click(selector)


    def click_link_by_class(self, class_name):
        selector = f'//li[contains(@class, "{class_name}")]/a'
        self.page.wait_for_selector(selector,timeout=Config.ELEMEMT_SEARCH_TIMEOUT_S)
        self.page.click(selector)

    def assert_text_present(self, text: str, tag: str = None):
        """
        Проверяет, что на странице есть элемент с указанным текстом.
        Если указан тег, проверяет именно его, иначе ищет любой элемент.
        """
        if tag:
            locator = self.page.locator(f'{tag}:has-text("{text}")')
        else:
            locator = self.page.locator(f'text="{text}"')
        expect(locator).to_be_visible()

    def assert_pdf_opened(self, expected_pdf_name: str):
        pattern = re.compile(re.escape(expected_pdf_name) + r'$')
        expect(self.page).to_have_url(pattern)

    def navigate_to_home(self):
        self.page.click('//a[h1[text()="Факультет прикладной математики и кибернетики"]]', force=True)

    def assert_on_home(self):
        # Проверяем, что URL стал равен главной странице
        expect(self.page).to_have_url(Config.BASE_URL)

    def click_priem_banner(self):
        self.page.click('a[href="http://priem.tversu.ru/"]')

    def assert_text_in_element(self, selector: str, expected_text: str, timeout: int = 5000):
        """
        selector: CSS или XPath элемента
        expected_text: текст, который должен быть внутри элемента
        timeout: время ожидания появления элемента
        """
        # Ждём элемент
        element = self.page.wait_for_selector(selector, timeout=timeout)
        # Получаем текст и проверяем наличие
        element_text = element.inner_text()
        assert expected_text in element_text, f"Ожидалось наличие текста '{expected_text}', а найдено '{element_text}'"