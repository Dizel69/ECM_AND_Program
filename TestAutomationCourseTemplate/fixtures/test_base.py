import pytest
import json
import os
from pathlib import Path

class BaseTest:
    test_data = None

    @pytest.fixture(autouse=True, scope="class")
    def class_setup_teardown(self):
        # Выполняется один раз перед всеми тестами в классе
        data_path = Path(__file__).parent.parent / "fixtures" / "test_data.json"
        with open(data_path, encoding="utf-8") as f:
            self.__class__.test_data = json.load(f)
        yield
        # Действия после завершения всех тестов в классе (если нужны)

    @pytest.fixture(autouse=True)
    def method_setup_teardown(self, page):
        # Выполняется перед каждым тестовым методом
        self.page = page
        yield
        # Выполняется после каждого тестового метода
        if hasattr(self, 'page') and self.page.context:
            try:
                self.page.screenshot(path=f"reports/screenshots/{self.__class__.__name__}_{self._testMethodName}.png")
            except Exception:
                pass