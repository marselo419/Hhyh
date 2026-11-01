#include <Geode/Geode.hpp>
#include <Geode/modify/GJBaseGameLayer.hpp>

using namespace geode::prelude;

class $modify(AssistLayer, GJBaseGameLayer) {
    struct Fields {
        bool pending = false;
        bool injecting = false;
        int framesLeft = 0;
    };

    void handleButton(bool down, int button, bool isPlayer1) {
        auto f = m_fields.self();
        if (!f->injecting && button == 1 && isPlayer1) {
            if (down && m_player1 && !m_player1->m_isOnGround) {
                // Click adelantado: lo guardamos unos frames
                f->pending = true;
                f->framesLeft = Mod::get()->getSettingValue<int64_t>("buffer-frames");
            } else if (!down) {
                // Soltaste antes de aterrizar: cancelamos
                f->pending = false;
            }
        }
        GJBaseGameLayer::handleButton(down, button, isPlayer1);
    }

    void update(float dt) {
        auto f = m_fields.self();
        if (f->pending) {
            if (m_player1 && m_player1->m_isOnGround) {
                f->pending = false;
                f->injecting = true;
                GJBaseGameLayer::handleButton(false, 1, true);
                GJBaseGameLayer::handleButton(true, 1, true);
                f->injecting = false;
            } else if (--f->framesLeft <= 0) {
                f->pending = false;
            }
        }
        GJBaseGameLayer::update(dt);
    }
};
