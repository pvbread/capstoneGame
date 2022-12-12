#include "TextBox.h"

TextBox::TextBox(std::string text,
                 int fontSize,
                 int x, int y,
                 int w, int h,
                 std::string fontPath, 
                 SDL_Color textColor, 
                 SDL_Color backgroundColor,
                 bool isClear)
{
    this->text = text;
    this->fontSize = fontSize;
    this->textBox = {x, y, w, h};
    this->textColor = textColor;
    this->backgroundColor = backgroundColor;
    this->isClear = isClear;
    font = TTF_OpenFont(fontPath.c_str(), fontSize);
}

void TextBox::render(SDL_Renderer* renderer)
{
    std::stringstream textStream;
    textStream << text;
    TTF_SizeText(font, text.c_str(), &textBox.w, &textBox.h);
    SDL_Surface* surface;
    
    if (isClear)
    {
        SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND); 
        SDL_SetRenderDrawColor(renderer, backgroundColor.r, backgroundColor.g, backgroundColor.b, 0);

        SDL_RenderFillRect(renderer, &textBox);
        surface = TTF_RenderUTF8_Blended(font, textStream.str().c_str(), textColor); 
    }
    else
    {
        SDL_SetRenderDrawColor(renderer, backgroundColor.r, backgroundColor.g, backgroundColor.b, backgroundColor.a);
        SDL_RenderFillRect(renderer, &textBox);
    
        surface = TTF_RenderUTF8_Blended(font, textStream.str().c_str(), textColor); 
    }
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    
    SDL_RenderCopy(renderer, texture, nullptr, &textBox); 

    SDL_FreeSurface(surface);
    SDL_DestroyTexture(texture);
    if (isClear)
        SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_NONE); 
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

int TextBox::getBottomY() const
{
    return textBox.y + textBox.h;
}