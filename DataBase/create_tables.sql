CREATE TABLE Manufacturer (
    Manufacturer_ID SERIAL PRIMARY KEY,
    Name VARCHAR(100) NOT NULL,
    Country VARCHAR(100) NOT NULL
);

CREATE TABLE Medicine (
    Medicine_ID SERIAL PRIMARY KEY,
    Name VARCHAR(100) NOT NULL,
    Form VARCHAR(50),
    Dosage VARCHAR(50),
    Manufacturer_ID INT NOT NULL,
    FOREIGN KEY (Manufacturer_ID) REFERENCES Manufacturer(Manufacturer_ID)
);

CREATE TABLE Application (
    Application_ID SERIAL PRIMARY KEY,
    Medicine_ID INT NOT NULL,
    Disease VARCHAR(100) NOT NULL,
    Usage_method VARCHAR(100),
    FOREIGN KEY (Medicine_ID) REFERENCES Medicine(Medicine_ID)
);

CREATE TABLE Pharmacy (
    Pharmacy_ID SERIAL PRIMARY KEY,
    Name VARCHAR(100) NOT NULL,
    Address VARCHAR(200) NOT NULL
);

CREATE TABLE Sale (
    Sale_ID SERIAL PRIMARY KEY,
    Medicine_ID INT NOT NULL,
    Pharmacy_ID INT NOT NULL,
    Sale_date DATE NOT NULL,
    Price DECIMAL(10, 2) NOT NULL,
    FOREIGN KEY (Medicine_ID) REFERENCES Medicine(Medicine_ID),
    FOREIGN KEY (Pharmacy_ID) REFERENCES Pharmacy(Pharmacy_ID)
);
