
/// \file line_view.hpp
/// Contains definition of the line_view class.

#include "../concepts.hpp"
#include "../ref.hpp"
#include <QLineEdit>


namespace mv::qt {


/// Read only line view control for text model containing std::string or std::wstring
template <viewable_observable Observable>
requires viewable_observable_as<Observable, std::string> || viewable_observable_as<Observable, std::wstring>
class ro_line_view: public QLineEdit {
public:
    /// Constructs line view with specified reference to model and parent widget
    ro_line_view(Observable mdl, QWidget * parent = nullptr):
    QLineEdit{parent},
    mdl_{mdl} {
        setReadOnly(true);
        con_ = mdl_.after_changed.connect([this] {
            update_value();
        });

        update_value();
    }

protected:
    /// Updates line edit value with value from model
    void update_value() {
        QString s = get_model_val();
        if (s != text()) {
            setText(s);
        }
    }

private:
    /// Returns QString containing in model
    QString get_model_val() const {
        if constexpr (observable_as<Observable, std::string>) {
            return QString::fromStdString(mdl_.get());
        } else if constexpr (observable_as<Observable, std::wstring>) {
            return QString::fromStdWString(mdl_.get());
        }
    }

    Observable mdl_;                    ///< Model
    scoped_signal_connection con_;      ///< Connection to model signal
};


/// Line view control for text model containing std::string or std::wstring
template <viewable_observable Model>
requires viewable_model<Model, std::string> || viewable_model<Model, std::wstring>
class line_view: public ro_line_view<Model> {
public:
    /// Constructs line view with specified reference to model and parent widget
    line_view(Model mdl, QWidget * parent = nullptr):
    ro_line_view<Model>{mdl, parent},
    mdl_{mdl} {
        this->setReadOnly(false);

        this->connect(this, &QLineEdit::textChanged, [this] {
            if constexpr (model<Model, std::string>) {
                mdl_.set(this->text().toStdString());
            } else if constexpr (model<Model, std::wstring>) {
                mdl_.set(this->text().toStdWString());
            }

            // resulting value in model may be different from value in line edit.
            // We need update line edit value in such case.
            this->update_value();
        });
    }

private:
    Model mdl_;                         ///< Model
};


template <viewable_observable Model>
line_view(Model && mdl) -> line_view<views::all_t<Model>>;


}
