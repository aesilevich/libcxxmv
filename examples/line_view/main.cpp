
#include "cxxmv/basic_model.hpp"
#include "cxxmv/ref.hpp"
#include "cxxmv/qt/line_view.hpp"
#include "cxxmv/transform.hpp"
#include <QApplication>
#include <QMainWindow>
#include <QFormLayout>
#include <QPushButton>
#include <QVBoxLayout>
#include <iostream>
#include <sstream>



struct contact {
    std::wstring first_name;
    std::wstring last_name;

    bool operator==(const contact & other) const {
        return first_name == other.first_name && last_name == other.last_name;
    }
};


class main_window: public QMainWindow {
public:
    main_window() {
        auto body = new QWidget;
        setCentralWidget(body);

        auto * layout = new QFormLayout{body};

        auto get_first_name = [](const contact & c) { return c.first_name; };
        auto get_last_name = [](const contact & c) { return c.last_name; };
        auto set_first_name = [](contact & c, const std::wstring & f_name) { c.first_name = f_name; };
        auto set_last_name = [](contact & c, const std::wstring & l_name) { c.last_name = l_name; };

        auto get_full_name = [](const contact & c) { return c.first_name + L" " + c.last_name; };

        auto f_name = new mv::qt::line_view{mdl_ | mv::views::transform(get_first_name, set_first_name)};
        layout->addRow("First name:", f_name);

        auto l_name = new mv::qt::line_view{mdl_ | mv::views::transform(get_last_name, set_last_name)};
        layout->addRow("Last name:", l_name);

        auto full_name = new mv::qt::ro_line_view{mdl_ | mv::views::transform(get_full_name)};
        layout->addRow("Full name:", full_name);
    }

private:
    mv::basic_model<contact> mdl_{{L"Jonh", L"Smith"}};
};


int main(int argc, char * argv[]) {
    try {
        QApplication app{argc, argv};
        main_window wnd;
        wnd.show();
        return app.exec();
    } catch (std::exception & err) {
        std::cerr << "ERROR: " << err.what() << "\n";
    }
    return 0;
}
