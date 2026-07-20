#ifndef SNAKEMANAGER_H
#define SNAKEMANAGER_H

#include <QAbstractListModel>
#include <QCoreApplication>
#include <QPoint>
#include <QTimer>
#include <QQmlEngine>
#include <QVector>

class SnakeManager : public QAbstractListModel
{
    Q_OBJECT

    Q_PROPERTY(QString message READ message WRITE setMessage NOTIFY messageChanged)
    Q_PROPERTY(QPoint food READ food WRITE setFood NOTIFY foodChanged)

    QML_ELEMENT

public:
    enum Roles
    {
        PositionRole = Qt::UserRole + 1
    };

    enum class Direction
    {
        Left,
        Up,
        Right,
        Down
    };

    explicit SnakeManager(QObject *parent = nullptr);

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role) const override;
    QHash<int, QByteArray> roleNames() const override;

    Q_INVOKABLE void changeDirection(int key);
    Q_INVOKABLE void reset();
    Q_INVOKABLE void requestQuit();

    QString message() const;
    void setMessage(const QString &message);

    QPoint food() const;
    void setFood(const QPoint &food);

signals:
    void messageChanged();
    void foodChanged();

private:
    static constexpr int CellSize = 20;
    static constexpr int BoardSize = 640;
    static constexpr int BoardCells = BoardSize / CellSize;
    static constexpr int TickInterval = 100;

    void moveSnake(int deltaX, int deltaY);

    void clearSnake();
    void addSnakeSegment(const QPoint &position);

    QPoint generateFoodPosition() const;
    bool isOccupied(const QPoint &position) const;

private:
    QVector<QPoint> m_snake;
    QPoint mFood;

    QTimer mTimer;

    QString mMessage;

    Direction mDirection = Direction::Right;
    Direction mPendingDirection = Direction::Right;
};

#endif // SNAKEMANAGER_H
