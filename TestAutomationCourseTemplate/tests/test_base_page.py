import pytest
import allure

from playwright.sync_api import Page, expect
from utils.config import Config
from pages.header import PMKPage

@allure.feature("Проверка возвращения на главную страницу")
def test_navigate_home(page: Page):
    header = PMKPage(page)
    header.open(Config.BASE_URL)
    header.click_link_by_text("Контакты")
    header.navigate_to_home()
    header.assert_on_home()

@allure.feature("Скачивание документа расписания")
def test_download_docs(page: Page):
    header = PMKPage(page)
    header.open(Config.BASE_URL)
    header.click_link_by_text("Студентам")
    header.click_link_by_text("Расписание занятий")
    header.click_link_by_text("расписание")
    header.assert_pdf_opened("663.pdf")

@allure.feature("Поиск сотрудников кафедры информатики")
def test_department_staff(page: Page):
    header = PMKPage(page)
    header.open(Config.BASE_URL)
    header.click_link_by_text("Кафедры")
    header.click_link_by_text("Кафедра информатики")
    header.click_link_by_text("Сотрудники")
    assert page.locator('//td/a[text()="Соловьев Иван Алексеевич"]').is_visible()

@allure.feature("Поиск контактов")
def test_contact(page: Page):
    header = PMKPage(page)
    header.open(Config.BASE_URL)
    header.click_link_by_text("Контакты")
    header.assert_text_present("Контакты", tag="h1")
    
@allure.feature("Проверка поиска научных публикаций преподавателя")
def test_publications(page: Page):
    header = PMKPage(page)
    header.open(Config.BASE_URL)
    header.click_link_by_text("Кафедры")
    header.click_link_by_text("Кафедра информатики")
    header.click_link_by_text("Сотрудники")
    header.click_link_by_text("Дудаков Сергей Михайлович")
    header.assert_text_present("Научные публикации:", tag="h2")

@allure.feature("Переход на сайт приёмной комиссии")
def test_admissions_committee(page: Page):
    header = PMKPage(page)
    header.open(Config.BASE_URL)
    header.click_priem_banner()
    header.assert_text_in_element("h3.tvsu-priem-header__subtitle", "правильный")