#include "TextBox.h"

TextBox::TextBox(std::string text,
                 int fontSize,
                 int x, int y,
                 int w, int h,
                 std::string fontPath, 
                 SDL_Color textColor, 
                 SDL_Color backgroundColor)
{
    this->text = text;
    this->fontSize = fontSize;
    this->textBox = {x, y, w, h};
    this->textColor = textColor;
    this->backgroundColor = backgroundColor;
    font = TTF_OpenFont(fontPath.c_str(), fontSize);
}

void TextBox::render(SDL_Renderer* renderer)
{
    SDL_SetRenderDrawColor(renderer, backgroundColor.r, backgroundColor.g, backgroundColor.b, backgroundColor.a);
    SDL_RenderFillRect(renderer, &textBox);
    std::stringstream textStream;
    textStream << text;
    TTF_SizeText(font, text.c_str(), &textBox.w, &textBox.h);
    SDL_Surface* surface = TTF_RenderUTF8_Blended(font, textStream.str().c_str(), textColor); 
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface); 
    SDL_RenderCopy(renderer, texture, nullptr, &textBox); 

    SDL_FreeSurface(surface);
    SDL_DestroyTexture(texture);
}

void TextBox::changeText(std::string text)
{
    this->text = text;
}

void TextBox::changeDimensions(int w, int h)
{
    textBox.w = w;
    textBox.h = h;
}
void TextBox::changePosition(int x, int y)
{
    textBox.x = x;
    textBox.y = y;
}
void TextBox::changeTextColor(SDL_Color color)
{
    textColor = color;
}
void TextBox::changeBackgroundColor(SDL_Color color)
{
    backgroundColor = color;
}