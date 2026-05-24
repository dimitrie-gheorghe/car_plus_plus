//
// Created by dimitrie on 23.05.2026.
//

#include "EditableText.h"
#include "./VertexArrayUtility.h"

void EditableText::updateVertexArray() {
        textVertexArray.clear();
        textVertexArray.setPrimitiveType(sf::PrimitiveType::Triangles);

        backgroundVertexArray.clear();
        backgroundVertexArray.setPrimitiveType(sf::PrimitiveType::Triangles);
        VertexArrayUtility::insertRectangle(backgroundVertexArray, x, y, static_cast<int16_t>(W),
                                            static_cast<int16_t>(H), backgroundColor);

        std::string formattedText = text;
        const size_t charPoz = 9 * pixelSize;
        const size_t rightLimit = W + x - charPoz;
        step = (W + x) / charPoz;

        size_t formattedCursorL = cursorL;
        size_t formattedCursorR = cursorR;

        size_t i = 0;
        while (i < formattedText.size()) {
            if (formattedText[i] == '\t') {
                if (i < formattedCursorL) {
                    formattedCursorL += 3;
                }
                if (i < formattedCursorR) {
                    formattedCursorR += 3;
                }
                formattedText[i] = ' ';
                formattedText.insert(i, "   ");
                i += 4;
            } else if (formattedText[i] == '\n') {
                const size_t old_i = i;
                formattedText[i] = ' ';
                i++;
                while (i % step) {
                    formattedText.insert(i, " ");
                    i++;
                }
                if (old_i < formattedCursorL) {
                    formattedCursorL += i - old_i - 1;
                }
                if (old_i < formattedCursorR) {
                    formattedCursorR += i - old_i - 1;
                }
            } else {
                i++;
            }
        }

        if (formattedCursorR == formattedText.size()) {
            formattedText += " "; // the space helps to display the cursor if it is at the end of the text
        }

        size_t visibleTextL = 0;
        size_t visibleTextR = formattedText.size();

        // if the text doesn't fit the window
        const size_t maxChars = (W * H) / (charPoz * charPoz);
        size_t focusCursor;
        if (selectionHead == &cursorL) {
            focusCursor = formattedCursorL;
        } else {
            focusCursor = formattedCursorR;
        }
        while (focusCursor - visibleTextL + step >= maxChars) {
            visibleTextL += step;
        }
        if (visibleTextL + maxChars < visibleTextR) {
            visibleTextR = visibleTextL + maxChars;
        }

        for (size_t r = y; visibleTextL < visibleTextR; r += charPoz) {
            for (size_t c = x; c <= rightLimit && visibleTextL < visibleTextR; c += charPoz) {
                if (formattedCursorL <= visibleTextL && visibleTextL <= formattedCursorR) {
                    // This happens if the char has cursor focus
                    VertexArrayUtility::insertRectangle(textVertexArray, static_cast<int16_t>(c),
                                                        static_cast<int16_t>(r), static_cast<int16_t>(pixelSize * 9),
                                                        static_cast<int16_t>(pixelSize * 9), cursorColor);
                }
                if ('!' <= formattedText[visibleTextL] && formattedText[visibleTextL] <= '~') {
                    VertexArrayUtility::insertChar(textVertexArray, formattedText[visibleTextL],
                                                   static_cast<int16_t>(c), static_cast<int16_t>(r), pixelSize,
                                                   textColor);
                }

                visibleTextL++;
            }
        }
    }

void EditableText::deleteSelectedText() {
    if (cursorL != cursorR) {
        text.erase(cursorL, cursorR - cursorL + 1);
        cursorR = cursorL;
    }
}

int EditableText::draw() {
    window->draw(backgroundVertexArray);
    window->draw(textVertexArray);
    return 0;
}

int EditableText::manageOtherEvents() {
    if (const std::optional<sf::Event> event = getEventForDerivedClasses(); event->getIf<sf::Event::Resized>()) {
        manageResizedEvent();
    } else if (const auto *textEntered = event->getIf<sf::Event::TextEntered>()) {
        manageEnteredText(textEntered);
    } else if (const auto *keyPressed = event->getIf<sf::Event::KeyPressed>()) {
        if (keyPressed->scancode == sf::Keyboard::Scancode::LControl || keyPressed->scancode ==
            sf::Keyboard::Scancode::RControl) {
            ctrl = true;
            } else if (keyPressed->scancode == sf::Keyboard::Scancode::LShift || keyPressed->scancode ==
                       sf::Keyboard::Scancode::RShift) {
                shift = true;
                       } else {
                           manageKey(keyPressed->scancode);
                       }
    } else if (const auto *keyReleased = event->getIf<sf::Event::KeyReleased>()) {
        if (keyReleased->scancode == sf::Keyboard::Scancode::LControl || keyReleased->scancode ==
            sf::Keyboard::Scancode::RControl) {
            ctrl = false;
            } else if (keyReleased->scancode == sf::Keyboard::Scancode::LShift || keyReleased->scancode ==
                       sf::Keyboard::Scancode::RShift) {
                shift = false;
                       }
    }
    return 0;
}

void EditableText::manageResizedEvent() {
    //reflow the text
    W = window->getSize().x;
    H = window->getSize().y;
    updateVertexArray();
}

void EditableText::manageEnteredText(const sf::Event::TextEntered *enteredText) {
    std::string c;
    if (enteredText->unicode < 128) {
        if (const char k = static_cast<char>(enteredText->unicode); k == 8) {
            if (!text.empty()) {
                if (cursorL && cursorL == cursorR) {
                    text.erase(--cursorL, 1);
                    cursorR = cursorL;
                } else {
                    deleteSelectedText();
                }
            }
        } else {
            c = " ";
            c[0] = k;
        }
    } else {
        c = " ";
        c[0] = 8;
        // 8 == backspace. This manages to insert the "unknown char" symbol for any other char than those dealt with above
    }
    if (!c.empty()) {
        deleteSelectedText();

        text.insert(cursorR++, c);
        cursorL++;
    }
    updateVertexArray();
}

int EditableText::manageKey(const sf::Keyboard::Scancode k) {
        bool updateRequired = false;
        size_t newCursor;
        if (selectionHead == &cursorR) {
            newCursor = cursorR;
        } else {
            newCursor = cursorL;
        }
        if (k == sf::Keyboard::Scancode::Right) {
            if (*selectionHead + 1 <= text.size()) {
                newCursor++;
            }
        } else if (k == sf::Keyboard::Scancode::Left) {
            if (static_cast<int16_t>(*selectionHead - 1) >= 0) {
                newCursor--;
            }
        } else if (k == sf::Keyboard::Scancode::Up) {
            int left = static_cast<int>(newCursor);
            size_t l = 0;
            if (left >= 0) {
                left--;
                l++;
            }
            while (left >= 0 && text[left] != '\n') {
                left--;
                l++;
            }
            const size_t former_l = l;

            int tmpCursor = static_cast<int>(newCursor);
            // It is very important to allow negative values (actually -1)
            for (uint8_t i = 0; i < 2; i++) {
                if (tmpCursor > -1) {
                    tmpCursor--;
                }
                while (tmpCursor > -1 && text[tmpCursor] != '\n') {
                    tmpCursor--;
                }
            }
            if (tmpCursor < static_cast<int>(text.size()) && l) {
                tmpCursor++;
                l--;
            }
            while (tmpCursor < static_cast<int>(text.size()) && text[tmpCursor] != '\n' && l) {
                tmpCursor++;
                l--;
            }
            if (tmpCursor + 1 == static_cast<int>(former_l) && tmpCursor == static_cast<int>(newCursor)) {
                newCursor = 0;
            } else {
                newCursor = static_cast<size_t>(tmpCursor);
            }
        } else if (k == sf::Keyboard::Scancode::Down) {
            int left = static_cast<int>(newCursor);
            size_t l = 0;
            if (left >= 0) {
                left--;
                l++;
            }
            while (left >= 0 && text[left] != '\n') {
                left--;
                l++;
            }

            if (text[newCursor] != '\n') {
                if (newCursor < text.size()) {
                    (newCursor)++;
                }
                while (newCursor < text.size() && text[newCursor] != '\n') {
                    (newCursor)++;
                }
            }
            if (newCursor < text.size() && l) {
                (newCursor)++;
                l--;
            }
            while (newCursor < text.size() && text[newCursor] != '\n' && l) {
                (newCursor)++;
                l--;
            }
        }
        if (newCursor != *selectionHead) {
            if (shift) {
                if (selectionHead == &cursorL) {
                    cursorL = newCursor;
                } else {
                    cursorR = newCursor;
                }
                if (cursorL > cursorR) {
                    size_t _ = cursorL;
                    cursorL = cursorR;
                    cursorR = _;
                    if (selectionHead == &cursorL) {
                        selectionHead = &cursorR;
                    } else {
                        selectionHead = &cursorL;
                    }
                }
            } else {
                if (cursorL == cursorR) {
                    cursorL = newCursor;
                    cursorR = newCursor;
                } else {
                    if (k == sf::Keyboard::Scancode::Left || k == sf::Keyboard::Scancode::Up) {
                        cursorR = cursorL;
                    } else {
                        cursorL = cursorR;
                    }
                }
            }

            updateRequired = true;
        } else if (!shift) {
            if (k == sf::Keyboard::Scancode::Left || k == sf::Keyboard::Scancode::Up) {
                cursorR = cursorL;
                updateRequired = true;
            } else if (k == sf::Keyboard::Scancode::Right || k == sf::Keyboard::Scancode::Down) {
                cursorL = cursorR;
                updateRequired = true;
            }
        }
        if (!shift && ctrl) {
            if (k == sf::Keyboard::Scancode::X) {
                // cut
                if (cursorL != cursorR) {
                    sf::Clipboard::setString(text.substr(cursorL, cursorR - cursorL + 1));
                }
                deleteSelectedText();

                updateRequired = true;
            } else if (k == sf::Keyboard::Scancode::C) {
                // copy
                if (cursorL != cursorR) {
                    sf::Clipboard::setString(text.substr(cursorL, cursorR - cursorL + 1));
                }
            } else if (k == sf::Keyboard::Scancode::V) {
                // paste
                const std::string s = sf::Clipboard::getString();
                if (!s.empty()) {
                    deleteSelectedText();
                    text.insert(cursorL, s);
                    cursorL += s.size();
                    cursorR = cursorL;

                    updateRequired = true;
                }
            } else if (k == sf::Keyboard::Scancode::A) {
                // select all
                cursorL = 0;
                cursorR = text.size();

                updateRequired = true;
            }
        }
        if (updateRequired) {
            updateVertexArray();
        }
        return 0;
    }

