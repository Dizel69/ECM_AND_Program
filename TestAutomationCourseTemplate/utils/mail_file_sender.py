import smtplib
import os
from email.mime.multipart import MIMEMultipart
from email.mime.text import MIMEText
from email.mime.base import MIMEBase
from email import encoders
from pathlib import Path

class MailReporter:
    SMTP_SERVER = os.getenv("SMTP_SERVER", "smtp.gmail.com")
    SMTP_PORT = int(os.getenv("SMTP_PORT", "587"))
    EMAIL = os.getenv("REPORT_EMAIL")
    PASSWORD = os.getenv("REPORT_EMAIL_PASSWORD")
    RECIPIENTS = os.getenv("REPORT_RECIPIENTS", "").split(",")

    @classmethod
    def send_report(cls, report_path: str, subject: str = "UI Test Report"):
        if not cls.EMAIL or not cls.PASSWORD or not cls.RECIPIENTS:
            return

        msg = MIMEMultipart()
        msg["From"] = cls.EMAIL
        msg["To"] = ", ".join(cls.RECIPIENTS)
        msg["Subject"] = subject

        body = "Attached is the latest test execution report."
        msg.attach(MIMEText(body, "plain"))

        report_file = Path(report_path)
        if report_file.exists():
            with open(report_file, "rb") as attachment:
                part = MIMEBase("application", "octet-stream")
                part.set_payload(attachment.read())
            encoders.encode_base64(part)
            part.add_header(
                "Content-Disposition",
                f'attachment; filename= {report_file.name}'
            )
            msg.attach(part)

        try:
            server = smtplib.SMTP(cls.SMTP_SERVER, cls.SMTP_PORT)
            server.starttls()
            server.login(cls.EMAIL, cls.PASSWORD)
            server.sendmail(cls.EMAIL, cls.RECIPIENTS, msg.as_string())
            server.quit()
        except Exception:
            pass