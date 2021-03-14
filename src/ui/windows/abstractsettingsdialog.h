/// @file abstractsettingsdialog.h
#ifndef ABSTRACTSETTINGSDIALOG_H
#define ABSTRACTSETTINGSDIALOG_H

#include <QDialog>
#include <QHBoxLayout>
#include <QStackedWidget>
#include <QStandardItemModel>

namespace Ui { class AbstractSettingsDialog; }

/**
 * @brief An abstract settings dialog.
 *
 * This dialog is used as the base class for various settings dialogs throughout
 * Schim. To properly subclass this dialog, you have to do the following.
 *
 * In the constructor:
 * - Add any settings categories you wish by calling `addCategory`
 * - Customize the stacked widget that contains the individual setting 'pages'
 * - Initialize the dialog by calling `initialize`
 *
 * The previous steps must be done in the specified order.
 *
 * After that, you can handle any events using the methods that this class
 * provides.
 *
 * @see `ProjectSettings`
 */
class AbstractSettingsDialog : public QDialog
{
    Q_OBJECT

public:
    // CONSTRUCTORS
    explicit AbstractSettingsDialog(QWidget *parent = nullptr);
    virtual ~AbstractSettingsDialog();

    // INITIALIZATION
    /**
     * @brief Add a category to the navigator.
     *
     * @note The categories will be displayed in the navigator in the same order
     * they have been added in.
     */
    void addCategory(const QString &name);
    /**
     * @brief After configuring the contents, call this to properly initialize
     * the dialog.
     */
    void initialize();

    // GETTERS
    QHBoxLayout *getTitleLayout();
    /**
     * @brief Get this dialog's stacked widget that you can customize.
     *
     * By default, the dialog comes with an empty stacked widget. This is the
     * widget that will contain the various pages for different
     * categories of settings.
     */
    QStackedWidget *getStackedWidget();

protected slots:
    // USER ACTIONS
    virtual void apply();
    virtual void accept() override;
    virtual void reject() override;

private slots:
    /**
     * @brief Called when a user changes a settings category.
     */
    void onCategorySelected(const QModelIndex &index);

private:
    // ATTRIBUTES
    Ui::AbstractSettingsDialog *ui;
    QStandardItemModel *model{};
};

#endif // ABSTRACTSETTINGSDIALOG_H
