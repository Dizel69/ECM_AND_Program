import os
from dotenv import load_dotenv

load_dotenv()

class Config:
    BASE_URL = os.getenv("BASE_URL", "http://pmk.tversu.ru/")
    ELEMEMT_SEARCH_TIMEOUT_S = float(os.getenv("ELEMEMT_SEARCH_TIMEOUT_S"))
    USERNAME = os.getenv("USERNAME")
    PASSWORD = os.getenv("PASSWORD")
    BROWSER = os.getenv("BROWSER", "chromium")
    HEADLESS = os.getenv("HEADLESS", "True").lower() == "true"