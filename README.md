# Basic Text Editor in C++ with FLTK

Welcome to the Basic Text Editor project! This project is a simple implementation of a text editor using C++ and the FLTK (Fast Light Toolkit) library. It serves as a learning project to understand the fundamentals of how code editors work and GUI programming in C++ with FLTK.

## Table of Contents
- [Introduction](#introduction)
- [Features](#features)
- [Requirements](#requirements)
- [Installation](#installation)
- [Usage](#usage)
- [Screenshots](#screenshots)
- [Contributing](#contributing)
- [License](#license)
- [Contact](#contact)

## Introduction

This project aims to provide a basic yet functional text editor. It includes fundamental features such as creating, opening, editing, and saving text files. The project is designed to help you learn and understand how to build GUI applications in C++ using the FLTK library.

## Features

- Create new text files
- Open existing text files
- Edit text
- Save text files
- Basic GUI with a menu and text area

## Requirements

To run this project, you'll need the following:

- C++ compiler (e.g., g++)
- FLTK library

## Installation

Follow these steps to set up the project on your local machine:

1. **Clone the repository:**
    ```sh
    git clone https://github.com/Law1212/text-editor.git
    cd text-editor
    ```

2. **Install FLTK:**
    - **Ubuntu:**
        ```sh
        sudo apt-get install libfltk1.3-dev
        ```
    - **MacOS:**
        ```sh
        brew install fltk
        ```
    - **Windows:**
        - Download and install FLTK from the [official website](https://www.fltk.org/).

3. **Compile the project:**
    ```sh
    g++ -o texteditor main.cpp `fltk-config --cxxflags --ldflags`
    ```

## Usage

To run the text editor, execute the following command:

```sh
./texteditor
