# multiuser terminal app templet
a outline for a server client system based entirely in the terminal. gives a nice stylized appearence.

__Game jam participants may find this useful.__

## features
- interactive UI thanks to <a href="https://github.com/ArthurSonzogni/FTXUI">FTXUI</a>
- network packet management based off of a incomplete project of <a href="https://www.youtube.com/c/javidx9">javidx9</a>s (allows for easy searlization & packet management)
- comes with some controls for server administration, self explainatory to add to
- capture portal for new connections
- network instance management system for `GameObject`s that helps support dynamic seralization and syncing.
- simple id range partitioning for clients to allow clients create instances and send info on their own tiem, iwthout server interaction.
- server indipendent of most client details. only package id's

<video width="100%" controls>
    <source src="demo.mp4" type="video/mp4">
</video>
<div style="text-align: right">
    <i>demo of server hosting a few clients. shows server host management clients display a horizontal red line showing dynamic screen dimnesions</i>
</div>

### use
- please credit <a href="https://github.com/ArthurSonzogni/FTXUI">FTXUI</a> and <a href="https://www.youtube.com/c/javidx9">javidx9</a> for their respective works and influences in your project

- This can ported to CMake if you would like; the only libs that need decalred are the asio standalone header library, ftxgui 5.0, and NetCommon (already part of this 
repository).

this follows a oop structure. the main class to concern yourself with is `GameObject` and its helper class `GameObjectFactory<T>`, these two are what would allow you to sync objects over the network; because of reflection limitations with c++ and my design choices, you must manually make a static instance of `GameObjectFactory<T>` for each decendent. *The idea is that `GameObjectFactory<T>` will generate a unique ID of the class at compile time that will be used by clients to dynamically instantiate a instance of said class using `GameObjectFactory::create(id)` which can be updated using `GameObject::unpackMessage(...)`*

There are some example classes that outline how objects may be managed in a game like manner, e.g `Grid`(aka a level), and others.