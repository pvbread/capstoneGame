#include "TextBox.h"

TextBox::TextBox(std::string text,
                 int fontSize,
                 int x, int y,
                 int w, int h,
                 std::string inputFont, 
                 SDL_Color textColor, 
                 SDL_Color backgroundColor)
{
    this->text = text;
    this->fontSize = fontSize;
    this->textBox = {x, y, w, h};
    this->textColor = textColor;
    this->backgroundColor = backgroundColor;
    std::string prefix = "./";
    std::string suffix = ".ttf";
    const char* filePath = (prefix + inputFont + suffix).c_str();
    font = TTF_OpenFont(filePath, fontSize);
}

void TextBox::render(SDL_Renderer* renderer)
{
    SDL_SetRenderDrawColor(renderer, backgroundColor.r, backgroundColor.g, backgroundColor.b, backgroundColor.a);
    SDL_RenderFillRect(renderer, &textBox);
    std::stringstream textStream;
    textStream << text;
    SDL_Surface* surface = TTF_RenderText_Solid(font, textStream.str().c_str(), textColor); 
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface); 
    SDL_RenderCopy(renderer, texture, nullptr, &textBox); 
}