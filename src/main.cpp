#include <SFML/Graphics.hpp>
#include <vector>
#include <cmath>
#include <cstring>
#include <string>
#include <cstdlib>

const int WIDTH = 1200, HEIGHT = 800;
const int COLS = 200, ROWS = 150;
const float DX = (float)WIDTH / COLS;
const float DY = (float)HEIGHT / ROWS;
const float C = 0.4f;  // wave speed
const float DAMPING = 0.995f;

float cur[ROWS][COLS] = {};
float prv[ROWS][COLS] = {};
float nxt[ROWS][COLS] = {};

void addDrop(int cx, int cy, float strength = 8.f, int radius = 6) {
    for (int r = cy - radius; r <= cy + radius; r++)
        for (int c = cx - radius; c <= cx + radius; c++) {
            if (r < 0 || r >= ROWS || c < 0 || c >= COLS) continue;
            float d = std::sqrt((float)((r-cy)*(r-cy)+(c-cx)*(c-cx)));
            if (d < radius)
                cur[r][c] += strength * std::cos(d / radius * 3.14159f * 0.5f);
        }
}

void stepWave() {
    for (int r = 1; r < ROWS-1; r++)
        for (int c = 1; c < COLS-1; c++) {
            nxt[r][c] = (2*cur[r][c] - prv[r][c] +
                C*C*(cur[r-1][c]+cur[r+1][c]+cur[r][c-1]+cur[r][c+1]-4*cur[r][c]))
                * DAMPING;
        }
    for (int r = 0; r < ROWS; r++)
        for (int c = 0; c < COLS; c++) {
            prv[r][c] = cur[r][c];
            cur[r][c] = nxt[r][c];
        }
}

sf::Color waveColor(float v, int mode) {
    float t = std::max(-1.f, std::min(1.f, v * 0.3f));
    if (mode == 0) {
        // Ocean blue
        uint8_t r = (uint8_t)(30 + (1-t)*20);
        uint8_t g = (uint8_t)(80 + t*80);
        uint8_t b = (uint8_t)(180 + t*75);
        return sf::Color(r, g, b);
    } else if (mode == 1) {
        // Lava
        float u = (t + 1.f) * 0.5f;
        return sf::Color(
            (uint8_t)(255 * std::min(1.f, u * 2)),
            (uint8_t)(255 * std::max(0.f, u * 2 - 1)),
            0
        );
    } else {
        // Psychedelic
        return sf::Color(
            (uint8_t)(127 + 127*std::sin(t*3.14f)),
            (uint8_t)(127 + 127*std::sin(t*3.14f + 2.09f)),
            (uint8_t)(127 + 127*std::sin(t*3.14f + 4.19f))
        );
    }
}

int main() {
    sf::RenderWindow window(
        sf::VideoMode({(unsigned)WIDTH, (unsigned)HEIGHT}),
        "Wave Simulation | LMB: Drop | Space: Clear | 1-3: Color | R: Rain | S: Storm"
    );
    window.setFramerateLimit(60);
    srand(42);

    // Pixel array
    std::vector<uint8_t> pixels(WIDTH * HEIGHT * 4, 0);
    sf::Texture tex({(unsigned)WIDTH, (unsigned)HEIGHT});
    sf::Sprite sprite(tex);

    int colorMode = 0;
    bool rain = false;
    float rainTimer = 0.f;
    bool storm = false;

    sf::Font font;
    bool hf = font.openFromFile("C:/Windows/Fonts/arial.ttf");
    sf::Text hud(font);
    hud.setCharacterSize(14);
    hud.setFillColor(sf::Color::White);
    hud.setOutlineColor(sf::Color::Black);
    hud.setOutlineThickness(1.5f);
    hud.setPosition({8.f, 8.f});

    sf::Clock clock;
    bool mouseDown = false;

    // Initial drops
    addDrop(COLS/2, ROWS/2, 10.f, 8);
    addDrop(COLS/4, ROWS/3, 8.f, 6);

    while (window.isOpen()) {
        float dt = std::min(clock.restart().asSeconds(), 0.02f);
        auto mp = sf::Mouse::getPosition(window);
        int mx = (int)(mp.x / DX), my = (int)(mp.y / DY);

        while (auto ev = window.pollEvent()) {
            if (ev->is<sf::Event::Closed>()) window.close();
            if (auto* k = ev->getIf<sf::Event::KeyPressed>()) {
                if (k->code == sf::Keyboard::Key::Escape) window.close();
                if (k->code == sf::Keyboard::Key::Space) {
                    memset(cur, 0, sizeof(cur));
                    memset(prv, 0, sizeof(prv));
                }
                if (k->code == sf::Keyboard::Key::Num1) colorMode = 0;
                if (k->code == sf::Keyboard::Key::Num2) colorMode = 1;
                if (k->code == sf::Keyboard::Key::Num3) colorMode = 2;
                if (k->code == sf::Keyboard::Key::R) rain = !rain;
                if (k->code == sf::Keyboard::Key::S) storm = !storm;
            }
            if (auto* mb = ev->getIf<sf::Event::MouseButtonPressed>())
                if (mb->button == sf::Mouse::Button::Left) mouseDown = true;
            if (auto* mb = ev->getIf<sf::Event::MouseButtonReleased>())
                if (mb->button == sf::Mouse::Button::Left) mouseDown = false;
        }

        if (mouseDown && mx >= 0 && mx < COLS && my >= 0 && my < ROWS)
            addDrop(mx, my, 6.f, 5);

        // Rain
        if (rain) {
            rainTimer += dt;
            if (rainTimer > 0.08f) {
                rainTimer = 0;
                int rc = rand() % COLS, rr = rand() % ROWS;
                addDrop(rc, rr, 3.f, 3);
            }
        }

        // Storm
        if (storm) {
            rainTimer += dt;
            if (rainTimer > 0.02f) {
                rainTimer = 0;
                for (int i = 0; i < 5; i++) {
                    int rc = rand() % COLS, rr = rand() % ROWS;
                    addDrop(rc, rr, 6.f, 5);
                }
            }
        }

        // Step simulation multiple times per frame
        for (int i = 0; i < 3; i++) stepWave();

        // Render pixels
        for (int r = 0; r < ROWS; r++) {
            for (int c = 0; c < COLS; c++) {
                sf::Color col = waveColor(cur[r][c], colorMode);
                int px = (int)(c * DX), py = (int)(r * DY);
                int pw = (int)DX + 1, ph = (int)DY + 1;
                for (int dy = 0; dy < ph && py+dy < HEIGHT; dy++)
                    for (int dx2 = 0; dx2 < pw && px+dx2 < WIDTH; dx2++) {
                        int idx = ((py+dy)*WIDTH + (px+dx2)) * 4;
                        pixels[idx]   = col.r;
                        pixels[idx+1] = col.g;
                        pixels[idx+2] = col.b;
                        pixels[idx+3] = 255;
                    }
            }
        }
        tex.update(pixels.data());

        window.clear();
        window.draw(sprite);

        if (hf) {
            std::string info =
                "LMB: Drop  |  Space: Clear  |  1:Ocean  2:Lava  3:Psychedelic  |  R:Rain(" +
                std::string(rain?"ON":"OFF") + ")  |  S:Storm(" +
                std::string(storm?"ON":"OFF") + ")";
            hud.setString(info);
            window.draw(hud);
        }

        window.display();
    }
    return 0;
}

