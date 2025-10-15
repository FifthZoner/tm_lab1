#include <SFML/Graphics.hpp>
#include <armadillo>

// oryginalne współrzędne, ustawiane dynamicznie
double xAp = 0;
double yAp = 0;

double xBp = 0;
double yBp = 0;

double xCp = 0;
double yCp = 0;

double xDp = 0;
double yDp = 0;

// margines
double offset = 10;

/// aktualizuje macierze po zmianie koordynatów
/// @param tex tekstura (dla wielkości)
/// @param sha shader
void Update(const sf::Texture& tex, sf::Shader& sha) {
    double H = tex.getSize().y;
    double W = tex.getSize().x;

    double xA = 0+offset;
    double yA = 0+offset;

    double xB = W-offset;
    double yB = 0+offset;

    double xC = W-offset;
    double yC = H-offset;

    double xD = 0+offset;
    double yD = H-offset;

    // wyliczanie macierzy przekształceń
    arma::mat P = {
        {xA, yA, 1, 0, 0, 0, -xA*xAp, -yA*xAp},
        {0, 0, 0, xA, yA, 1, -xA*yAp, -yA*yAp},
        {xB, yB, 1, 0, 0, 0, -xB*xBp, -yB*xBp},
        {0, 0, 0, xB, yB, 1, -xB*yBp, -yB*yBp},
        {xC, yC, 1, 0, 0, 0, -xC*xCp, -yC*xCp},
        {0, 0, 0, xC, yC, 1, -xC*yCp, -yC*yCp},
        {xD, yD, 1, 0, 0, 0, -xD*xDp, -yD*xDp},
        {0, 0, 0, xD, yD, 1, -xD*yDp, -yD*yDp}
    };

    arma::vec Q = {xAp, yAp, xBp, yBp, xCp, yCp, xDp, yDp};

    arma::vec R = arma::solve(P, Q);

    arma::mat M = {
        {R(0), R(1), R(2)},
        {R(3), R(4), R(5)},
        {R(6), R(7), 1}
    };

    sha.setUniform("r0", float(R(0)));
    sha.setUniform("r1", float(R(1)));
    sha.setUniform("r2", float(R(2)));
    sha.setUniform("r3", float(R(3)));
    sha.setUniform("r4", float(R(4)));
    sha.setUniform("r5", float(R(5)));
    sha.setUniform("r6", float(R(6)));
    sha.setUniform("r7", float(R(7)));
    sha.setUniform("r8", float(1));
}

// do prostszej obsługi rogów
enum Corner {
    a, b, c, d
};

double& GetX(Corner corner) {
    switch (corner) {
    case a: return xAp;
    case b: return xBp;
    case c: return xCp;
    case d: return xDp;
    default: throw std::exception();
    }
}

double& GetY(Corner corner) {
    switch (corner) {
    case a: return yAp;
    case b: return yBp;
    case c: return yCp;
    case d: return yDp;
    default: throw std::exception();
    }
}

int main() {
    // wczytywanie obrazu
    auto tex = sf::Texture("../img.png");
    // sprite do wyświeltlania obrazu przed/po
    auto spr = sf::Sprite(tex);

    // domyślne koordynaty punktów na rogach
    xAp = offset;
    yAp = tex.getSize().y - offset;
    xBp = tex.getSize().x - offset;
    yBp = tex.getSize().y - offset;
    xCp = tex.getSize().x - offset;
    yCp = offset;
    xDp = offset;
    yDp = offset;

    // wczytywanie fragment shadera, który wykona się dla każdego fragmentu (~piksela) obrazu na gpu
    auto sha = sf::Shader(std::filesystem::path("../shader.frag"), sf::Shader::Type::Fragment);
    // ustawianie przekazanych wartości
    sha.setUniform("texture", tex);
    sha.setUniform("sizeX", float(tex.getSize().x));
    sha.setUniform("sizeY", float(tex.getSize().y));
    Update(tex, sha);

    // tworzenie okien obrazu przed i po
    sf::RenderWindow window(sf::VideoMode(tex.getSize()), "Przed");
    sf::RenderWindow window2(sf::VideoMode(tex.getSize()), "Po");
    // limit klatek, żeby prędkość ruchu była stała, dostosowana do sprzętu na którym pisano
    window.setFramerateLimit(360);
    window2.setFramerateLimit(360);

    Corner corner = a;

    while (window.isOpen() and window2.isOpen())
    {
        while (const std::optional event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
                window.close();
            else if (event->is<sf::Event::KeyPressed>()) {
                auto key = event->getIf<sf::Event::KeyPressed>();
                switch (key->code) {
                case sf::Keyboard::Key::Num1: corner = a; break;
                case sf::Keyboard::Key::Num2: corner = b; break;
                case sf::Keyboard::Key::Num3: corner = c; break;
                case sf::Keyboard::Key::Num4: corner = d; break;
                    default:break;

                }
            }
        }
        while (const std::optional event = window2.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
                window2.close();
            else if (event->is<sf::Event::KeyPressed>()) {
                auto key = event->getIf<sf::Event::KeyPressed>();
                switch (key->code) {
                case sf::Keyboard::Key::Num1: corner = a; break;
                case sf::Keyboard::Key::Num2: corner = b; break;
                case sf::Keyboard::Key::Num3: corner = c; break;
                case sf::Keyboard::Key::Num4: corner = d; break;
                default:break;

                }
            }
        }


        auto& y = GetY(corner);
        auto& x = GetX(corner);
        double move = sf::Keyboard::isKeyPressed(sf::Keyboard::Key::LShift) ? 5 : 1;

        // przesuwanie punktów strzałkami
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up)) {
            y -= move;
            Update(tex, sha);
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Down)) {
            y += move;
            Update(tex, sha);
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left)) {
            x -= move;
            Update(tex, sha);
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right)) {
            x += move;
            Update(tex, sha);
        }

        // wyświetlanie obrazów przed, po i punktów
        window.clear();
        window2.clear();

        window2.draw(spr, &sha);
        window.draw(spr);
        sf::CircleShape point{};
        point.setRadius(6);
        point.setOrigin(sf::Vector2f(3.f, 3.f));
        point.setFillColor(corner == a ? sf::Color::Green : sf::Color::Red);
        point.setPosition(sf::Vector2f(xAp, yAp));
        window.draw(point);
                point.setFillColor(corner == b ? sf::Color::Green : sf::Color::Red);
        point.setPosition(sf::Vector2f(xBp, yBp));
        window.draw(point);
                point.setFillColor(corner == c ? sf::Color::Green : sf::Color::Red);
        point.setPosition(sf::Vector2f(xCp, yCp));
        window.draw(point);
                point.setFillColor(corner == d ? sf::Color::Green : sf::Color::Red);
        point.setPosition(sf::Vector2f(xDp, yDp));
        window.draw(point);

        window.display();
        window2.display();
    }
}