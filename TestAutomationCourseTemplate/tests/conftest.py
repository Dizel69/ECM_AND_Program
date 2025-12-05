import pytest
from playwright.sync_api import sync_playwright, Browser, Page
from utils.config import Config
from utils.mail_file_sender import MailReporter
import os

def pytest_sessionfinish(session, exitstatus):
    report_path = "reports/report.html"
    if os.path.exists(report_path):
        MailReporter.send_report(report_path)

@pytest.fixture(scope="session")
def browser():
    with sync_playwright() as p:
        browser_type = getattr(p, Config.BROWSER)
        browser = browser_type.launch(headless=Config.HEADLESS)
        yield browser
        browser.close()

@pytest.fixture
def page(browser):
    context = browser.new_context(viewport={'width': 1920, 'height': 1080})
    page = context.new_page()
    yield page
    context.close()