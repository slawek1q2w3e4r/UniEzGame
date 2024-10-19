#include <pybind11/pybind11.h>
#include <SDL.h>
#include <iostream>
#include <memory>
#include <cmath>  // Dla std::sqrt

namespace py = pybind11;

int dodaj(int a, int b) {
    return a + b;
}

// Wrapper dla SDL_Renderer
class RendererWrapper {
public:
    RendererWrapper(SDL_Renderer* renderer) : renderer(renderer) {}
    ~RendererWrapper() {
        if (renderer) {
            SDL_DestroyRenderer(renderer);
        }
    }

    void clear() {
        SDL_RenderClear(renderer);
    }

    void present() {
        SDL_RenderPresent(renderer);
    }

    SDL_Renderer* get() const { return renderer; }

private:
    SDL_Renderer* renderer;
};

// Klasa do rysowania
class Draw {
public:
    Draw(RendererWrapper& renderer) : renderer(renderer) {}

    // Funkcja do rysowania prostok¹tów
    void Rect(int x, int y, int width, int height, SDL_Color color) {
        SDL_SetRenderDrawColor(renderer.get(), color.r, color.g, color.b, color.a);  // Ustaw kolor rysowania
        SDL_Rect rect = { x, y, width, height };  // Tworzenie prostok¹ta
        SDL_RenderFillRect(renderer.get(), &rect);  // Rysowanie wype³nionego prostok¹ta
    }

    // Funkcja do rysowania okrêgów
    void Circle(int centerX, int centerY, int radius, SDL_Color color) {
        SDL_SetRenderDrawColor(renderer.get(), color.r, color.g, color.b, color.a);  // Ustaw kolor rysowania

        for (int w = 0; w < radius * 2; w++) {
            for (int h = 0; h < radius * 2; h++) {
                int dx = radius - w; // oblicz odleg³oœæ od œrodka
                int dy = radius - h;
                if ((dx * dx + dy * dy) <= (radius * radius)) {
                    SDL_RenderDrawPoint(renderer.get(), centerX + dx, centerY + dy); // Rysuj punkt
                }
            }
        }
    }

private:
    RendererWrapper& renderer; // Referencja do obiektu renderer
};

// Funkcja do tworzenia okna i ustawiania koloru t³a
std::shared_ptr<RendererWrapper> create_window(int width, int height, int r, int g, int b, const std::string& title) {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cerr << "Nie mo¿na zainicjowaæ SDL: " << SDL_GetError() << std::endl;
        return nullptr;
    }

    SDL_Window* window = SDL_CreateWindow(
        title.c_str(),
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        width,
        height,
        SDL_WINDOW_SHOWN
    );

    if (!window) {
        std::cerr << "Nie mo¿na stworzyæ okna: " << SDL_GetError() << std::endl;
        SDL_Quit();
        return nullptr;
    }

    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) {
        std::cerr << "Nie mo¿na stworzyæ renderera: " << SDL_GetError() << std::endl;
        SDL_DestroyWindow(window);
        SDL_Quit();
        return nullptr;
    }

    SDL_SetRenderDrawColor(renderer, r, g, b, 255);  // Ustaw kolor t³a
    SDL_RenderClear(renderer);  // Wyczyœæ ekran
    SDL_RenderPresent(renderer);  // Wyœwietl zmiany

    // Pêtla zdarzeñ, aby czekaæ na zamkniêcie okna
    bool running = true;
    SDL_Event event;
    while (running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = false;
            }
        }
        // Mo¿esz dodaæ tutaj inne operacje renderowania lub odœwie¿ania.
        SDL_Delay(16);  // Ma³e opóŸnienie dla zminimalizowania zu¿ycia CPU (60 FPS)
    }

    SDL_DestroyWindow(window);
    SDL_Quit();

    return std::make_shared<RendererWrapper>(renderer);
}

PYBIND11_MODULE(uniezgame, m) {
    m.doc() = "Prosta biblioteka C++ dla Pythona";
    m.def("dodaj", &dodaj, "Funkcja dodaj¹ca dwie liczby");

    py::class_<RendererWrapper, std::shared_ptr<RendererWrapper>>(m, "Renderer")
        .def("clear", &RendererWrapper::clear)
        .def("present", &RendererWrapper::present);

    py::class_<Draw>(m, "Draw")
        .def(py::init<RendererWrapper&>())  // Konstruktor przyjmuj¹cy RendererWrapper
        .def("rect", &Draw::Rect, "Rysuje prostok¹t",
            py::arg("x"), py::arg("y"),
            py::arg("width"), py::arg("height"),
            py::arg("color"))
        .def("circle", &Draw::Circle, "Rysuje okr¹g",
            py::arg("centerX"), py::arg("centerY"),
            py::arg("radius"),
            py::arg("color"));

    m.def("create_window", &create_window, "Funkcja tworz¹ca okno",
        py::arg("width"), py::arg("height"), py::arg("r"),
        py::arg("g"), py::arg("b"), py::arg("title"));
}
