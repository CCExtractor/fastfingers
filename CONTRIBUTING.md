# Contribute to FastFingers

There are several ways you can help improve FastFingers. You can contribute to the project by reporting issues,
making suggestions and modifying the code.

## Table of contents

* [Design](#design)
* [How to add new applications](#how-to-add-new-applications)
    + [Creating the JSON file](#creating-the-json-file)
        - [Application object:](#application-object-)
        - [Shortcut category object:](#shortcut-category-object-)
        - [Shortcut object:](#shortcut-object-)
        - [Finalizing](#finalizing)
    + [Adding the logo](#adding-the-logo)
    + [Adding the logo to GResource file](#adding-the-logo-to-gresource-file)
    + [Integrating your work with the project](#integrating-your-work-with-the-project)

## Design

FastFingers is a Linux GUI application. The main libraries and tools used in the project are GTK3, GLib, CMake, and cJSON.

The UI designs are stored under "src/ui" in the XML format, which is both human-readable and interpretable by
GtkBuilder.

FastFingers and Cheatsheet are two different executables. FastFingers starts from main.c and the Cheatsheet app starts
with cheatsheet.

Pages files are named "(pageName)-page", custom widget files take the widget's name. main.c, fastfingers.c and
cheatsheet.c files control the application flow. ff-utils.c file has the utility functions that are used in the
project.

Keyboard shortcuts are stored in the "data/applications" path. Each application has its JSON file in a custom
format. This format is described in the section below.

## How to add new applications

Application files are stored in the path "data/applications". If you want to add a new application, please read the
information below to understand the design. If you want to add a new application, you should create a .json file as
described, add its logo to "src/logo", and add that logo to
"org.ccextractor.FastFingers.gresource.xml". This progress is explained step by step below.

### Creating the JSON file

Each application has its JSON file in a custom format. The name of this JSON file should only include lower
case characters and shouldn't contain any whitespace.

#### Application object

```json
{
  "title": "Firefox",
  "category": "Utility",
  "recent": [],
  "group": [
    ...
  ]
}
```

* title
    * This key determines the title of the application. This is the name shown in the FastFingers for the application.
* category
    * This key determines the category of the application. On the home page, the applications are grouped under categories, so this information is important for harmony.
* recent
    * This key is used in the application run time. It should be left blank in the source.
* group
    * Shortcuts of the applications are stored under the categories, and the group key has an array value that holds those categories.

#### Shortcut category object

```json
{
  "title": "Navigation",
  "shortcuts": [
    ...
  ]
}
```

* title
    * This key determines the title of the shortcut category. This title is shown on the application screen.
* shortcuts
    * Shortcuts that belong to this category are stored in this array.

#### Shortcut object

```json
  {
  "title": "Back",
  "keys": [
    "Alt",
    "Left"
  ],
  "learned": 0
}
```

* title
    * Title is the both identifier and descriptor of the shortcut. In practice, quiz, and quiz result screens, this title is shown. Thi title should be as short as possible.
* keys
    * Keys that should be pressed are stored in this array in the same order.
* learned
    * Learned key is meaningful for the program and this key should be 0 in the source files.

#### Finalizing

After following the steps above, you should have a JSON structure like this:

```json
{
  "title": "Firefox",
  "category": "Utility",
  "recent": [],
  "group": [
    {
      "title": "Navigation",
      "shortcuts": [
        {
          "title": "Back",
          "keys": [
            "Alt",
            "Left"
          ],
          "learned": 0
        }
      ]
    }
  ]
}
```

To have a greater idea about the JSON files, you can take a look at existing files
at "[data/applications](https://github.com/CCExtractor/fastfingers/tree/main/data/applications)".

### Adding the logo

You should add the logo of the application to the
path "[src/logo](https://github.com/CCExtractor/fastfingers/tree/main/src/logo)". The name of the file should be in the same
format as the JSON file, it should consist of lower case characters, and it shouldn't contain whitespace. These logos
are shown in the home, application, practice, and quiz screens. The files are scaled automatically in the application, so
there is no size rule.

### Adding the logo to GResource file

The application holds the logos in its executable. To do this, you should define each logo in the GResource file
at "[src/org.ccextractor.FastFingers.gresource.xml](https://github.com/CCExtractor/fastfingers/blob/main/src/org.ccextractor.FastFingers.gresource.xml)"
.

```XML
    ...
<file>logo/fileName.png</file>
```

### Integrating your work with the project

You can send a pull request to integrate all your work with the project.
