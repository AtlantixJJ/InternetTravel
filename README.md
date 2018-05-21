# Internet Traffic

Database Special Topic Training course final project.

## Building

```
mkdir build && cd build
cmake ..
make
```

## Run

```
./main
```

## Run as web server

```
cd server
python manage.py runserver [0.0.0.0:PORT]
```

The default port is `8000`.

## Dependency

`zlib` in Ubuntu.