#ifndef UPLOAD_H
#define UPLOAD_H

#include <QWidget>

namespace Ui {
class Upload;
}

class Upload : public QWidget
{
    Q_OBJECT
    
public:
    explicit Upload(QWidget *parent = 0);
    ~Upload();
    
private:
    Ui::Upload *ui;
};

#endif // UPLOAD_H
