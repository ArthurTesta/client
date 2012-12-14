#ifndef UPLOAD_H
#define UPLOAD_H

#include <QWidget>

namespace Ui {
class Upload;
}
class TransferMessage;
class MainWindow;
class Upload : public QWidget
{
    Q_OBJECT
    
public:
    friend class MainWindow;
    explicit Upload(QWidget *parent = 0);
    ~Upload();
signals :
    void uploadSignal(QString * file,QString * desc);
private:
    QString fileName,filePath,fileDescription;
    int progress;
    Ui::Upload *ui;

private slots :
    void file();
    void createAction();
    void updateProgress(qint64 progress);
    void saveButtonClicked();
    void showUploadResult(TransferMessage * msg);
};

#endif // UPLOAD_H
