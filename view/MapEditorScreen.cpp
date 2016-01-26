#include <SFGUI/Canvas.hpp>
#include <SFGUI/Scrollbar.hpp>
#include <SFML/Graphics/View.hpp>
#include <SFGUI/ScrolledWindow.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFGUI/Adjustment.hpp>
#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Window/Mouse.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include "MapEditorScreen.h"
#include "SelectionTool.h"
#include "CountryCreationTool.h"
#include "RiskGameView.h"

MapEditorScreen::MapEditorScreen() {
    tools_["selection"] = new SelectionTool(&drawables_);
    tools_["countryCreation"] = new CountryCreationTool(&drawables_);
    currentTool_ = tools_["countryCreation"];
    initialize();
}

MapEditorScreen::~MapEditorScreen() {
    for (auto kv : tools_) {
        delete kv.second;
    }
}

void MapEditorScreen::initialize() {
    // Determine the initial sizes
    float canvasWidth = RiskGameView::Instance().GetWidth() * 1.272f;
    float canvasHeight = RiskGameView::Instance().GetHeight() / 2;

    // Create our SFML scrollable canvas window and canvas
    auto sfml_scrollable_window = sfg::Window::Create(sfg::Window::Style::BACKGROUND);
    sfml_scrollable_window->SetPosition(sf::Vector2f(0.0f, 0.0f));
    canvas_ = sfg::Canvas::Create();

    // Create a pair of scrollbars.
    auto horizontal_scrollbar = sfg::Scrollbar::Create(sfg::Scrollbar::Orientation::HORIZONTAL);
    auto vertical_scrollbar = sfg::Scrollbar::Create(sfg::Scrollbar::Orientation::VERTICAL);

    // Create a table to put the scrollbars and scrollable canvas in.
    auto table = sfg::Table::Create();
    table->Attach(canvas_, sf::Rect<sf::Uint32>(0, 0, 1, 1), sfg::Table::FILL | sfg::Table::EXPAND, sfg::Table::FILL | sfg::Table::EXPAND);
    table->Attach(vertical_scrollbar, sf::Rect<sf::Uint32>(1, 0, 1, 1), 0, sfg::Table::FILL);
    table->Attach(horizontal_scrollbar, sf::Rect<sf::Uint32>(0, 1, 1, 1), sfg::Table::FILL, 0);
    sfml_scrollable_window->Add(table);

    // Create an sf::View to control which part of our rectangle is drawn.
    view_= new sf::View(sf::Vector2f(canvasWidth/2, canvasHeight/2), sf::Vector2f(canvasWidth, canvasHeight));

    // Link the adjustments to our view and initialize arbitrary values
    horizontalAdjustment_ = horizontal_scrollbar->GetAdjustment();
    verticalAdjustment_ = vertical_scrollbar->GetAdjustment();
    horizontalAdjustment_->SetLower(canvasWidth / 2.f);
    horizontalAdjustment_->SetUpper(218.f * 20 - canvasWidth / 2.f);
    horizontalAdjustment_->SetMinorStep(1.f);
    horizontalAdjustment_->SetMajorStep(canvasWidth);
    horizontalAdjustment_->SetPageSize(canvasWidth);
    verticalAdjustment_->SetLower(canvasHeight / 2.f);
    verticalAdjustment_->SetUpper(84.f * 20 - canvasHeight / 2.f);
    verticalAdjustment_->SetMinorStep(1.f);
    verticalAdjustment_->SetMajorStep(canvasHeight);
    verticalAdjustment_->SetPageSize(canvasHeight);

    // Register to events
    canvas_->GetSignal(sfg::Window::OnLeftClick).Connect(std::bind(&MapEditorScreen::OnLeftClick, this));
    horizontalAdjustment_->GetSignal(sfg::Adjustment::OnChange).Connect(std::bind(&MapEditorScreen::OnHorizontalAdjustmentChanged, this));
    verticalAdjustment_->GetSignal(sfg::Adjustment::OnChange).Connect(std::bind(&MapEditorScreen::OnVerticalAdjustmentChanged, this));

    // Set map editor size
    canvas_->SetRequisition(sf::Vector2f(canvasWidth, canvasHeight));

    widgets_.push_back(sfml_scrollable_window);
    Screen::initialize();
}

void MapEditorScreen::Draw(sf::RenderTarget& target) const {
    canvas_->Bind();
    canvas_->Clear(sf::Color(33, 33, 33));

    canvas_->SetView(*view_);
    for (const sf::Drawable* drawable : drawables_) {
        canvas_->Draw(*drawable);
    }

    canvas_->Display();
    canvas_->Unbind();
}

void MapEditorScreen::OnLeftClick() {
    sf::Vector2i position = sf::Mouse::getPosition(*RiskGameView::Instance().GetRenderWindow());
    int x = (int)(position.x - canvas_->GetAbsolutePosition().x);
    int y = (int)(position.y - canvas_->GetAbsolutePosition().y);
    int horizontalScroll = (int)(horizontalAdjustment_->GetValue() - horizontalAdjustment_->GetPageSize() / 2);
    int verticalScroll = (int)(verticalAdjustment_->GetValue() - verticalAdjustment_->GetPageSize() / 2);
    currentTool_->Execute(x + horizontalScroll, y + verticalScroll);
}

void MapEditorScreen::OnHorizontalAdjustmentChanged() {
    view_->setCenter(horizontalAdjustment_->GetValue(), view_->getCenter().y);
}

void MapEditorScreen::OnVerticalAdjustmentChanged() {
    view_->setCenter(view_->getCenter().x, verticalAdjustment_->GetValue());
}