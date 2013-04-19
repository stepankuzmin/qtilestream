qtilestream
===========

CLI [Tilestream](https://github.com/mapbox/tilestream) on Qt\C++.

Download
--------

    git clone https://github.com/StepanKuzmin/QTileStream.git


Install
-------

    cd QTileStream
    qmake
    make

Usage
-----

    usage: qtilestream [--port=N] [--not-found-image=file] [--mbtiles=files] 

Example
-------

    ./qtilestream --port=8080 --not-found-image=404.png --mbtiles=sverdlovsk.mbtiles,world.mbtiles

Open `examples/openlayers.html` or `examples/leaflet.html` in your browser.
