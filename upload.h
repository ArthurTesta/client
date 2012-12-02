#ifndef UPLOAD_H
#define UPLOAD_H

#include <QWidget>

namespace Ui {
class Upload;
}
class MainWindow;
class Upload : public QWidget
{
    Q_OBJECT
    
public:
    friend class MainWindow;
    explicit Upload(QWidget *parent = 0);
    ~Upload();
    
private:
    int progress;
    Ui::Upload *ui;
private slots :
    void createAction();
    void updateProgress(qint64);
};

#endif // UPLOAD_H
