# LibreEDA

[![Build Status](https://travis-ci.org/chgans/LibreEDA.png?branch=master)](https://github.com/chgans/LibreEDA)

A (very experimental) [Electronics Design Automation](http://en.wikipedia.org/wiki/Electronic_design_automation)
[libre software](http://en.wikipedia.org/wiki/Free_software) based on
[the Qt5 C++ toolkit](http://www.qt.io), and maybe [KDE Framework 5](http://en.wikipedia.org/wiki/KDE_Frameworks_5) too!

## What's not in there?

LibreEDA was just born recently, so if you are looking for a usable open-source EDA, I am sad to let you know that
there no such thing (yet) in here. You might want to have a look
[here](http://www.gpleda.org) instead.

## Goals

*WIP*

This repository aims to provide such a piece of software, but it will take time,
and my approach is to start by writing a solid application framework before
adding any features you would expect from an EDA.


## Roadmap

*WIP*

See [milestones](https://github.com/chgans/LibreEDA/milestones) for up to date information.

* Multi editor/document support:

  Provide bolts and nuts to be able to open several document in different editors,
  right now, for the sake of simplicity, here are the planned simple editors:

  * Drawing editor (based on [Qt Graphics View Framework](http://doc-snapshots.qt.io/qt5-dev/graphicsview.html)):

    Allow the user to draw shape and line based element, load and save them to/from file.
    Follows the [TaskMaster](http://www.objectmentor.com/resources/articles/taskmast.pdf)
    architecture pattern.

  * Rich text editor (based on [Qt Text Document](http://doc-snapshots.qt.io/qt5-dev/qtextdocument.html):

    Allow the user to edit rich text file, load and save them to/from file

  * 3D viewer (based on brand new [Qt 3D](http://doc-snapshots.qt.io/qt5-dev/qt3drender-index.html)):

    Allow the user to visualise 3D files (file format to be defined)

* Project support:

  Once the application provides loading and saving documents of different formats using different
  kind of editors, the next step is to allow the user to group them in projects.

* Document model:

  A good document model for an EDA is one where the document is completely separated from the GUI.
  The document should be modified through a command based interface not only to provide undo/redo
  capability but to always insure repeatability. Any action performed graphically or not by the
  user should end up as a document command being executed, the GUI in turn shoud be updated
  according to the resulting changes (With the exception of the text document, which is
  quite a different thing, and this is exactly why i want it in the early editor list).

* Application extension:

  At this stage it should be possible to write pluggins to extend the application with new document models, editors and tools.

* Hierarchical design of reusable and configurable components

  This is where the real EDA core starts. I have been thinking about this for
  quite a while, checking how other (libre) software tackle this problem and
  which standard can be of any help. There is one standard/language that struck
  me with it's robustness and it's quite universal applicapability: [VHDL](en.wikipedia.org/wiki/VHDL)
  The idea is not to *use* VHDL as an internal data structure, but to follow it's basic concept of
  entity/component/implementation.


# EDA related libre software

TODO: Explain quickly what these software do, which technology they are based on, some pros and cons

See [Comparison of EDA Software](http://en.wikipedia.org/wiki/Comparison_of_EDA_Software)  on Wikipedia


## oregano

Simulation oriented schematic editor

## KiCad suite

Schematic editor, schematic symbol editor, footprint editor and PCB editor.

## gEDA suite

## qelectrotech

## qucs

Simulation oriented schematic editor

## tkgate

## electric VLSI designer

## logisim

## fritzing

## xcircuit
