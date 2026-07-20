#include "snakemanager.h"
#include <algorithm>
#include <random>

inline int getRandomNumber(int min, int max)
{
    static std::random_device rd;
    static std::mt19937 generator(rd());

    std::uniform_int_distribution<int> distribution(min, max);
    return distribution(generator);
}

SnakeManager::SnakeManager(QObject *parent)
    : QAbstractListModel(parent)
{
    reset();

    mTimer.setInterval(TickInterval);

    connect(&mTimer, &QTimer::timeout, this, [this]()
            {
                mDirection = mPendingDirection;

                switch (mDirection)
                {
                case Direction::Left:
                    moveSnake(-CellSize, 0);
                    break;

                case Direction::Up:
                    moveSnake(0, -CellSize);
                    break;

                case Direction::Right:
                    moveSnake(CellSize, 0);
                    break;

                case Direction::Down:
                    moveSnake(0, CellSize);
                    break;
                }
            });

    mTimer.start();
}


int SnakeManager::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return m_snake.size();
}


QVariant SnakeManager::data(const QModelIndex &index, int role) const
{
    if (!index.isValid() || index.row() >= m_snake.size())
        return {};

    if (role == PositionRole)
        return m_snake[index.row()];

    return {};
}


QHash<int, QByteArray> SnakeManager::roleNames() const
{
    return {
        {PositionRole, "position"}
    };
}


void SnakeManager::moveSnake(int deltaX, int deltaY)
{
    if (m_snake.isEmpty())
        return;


    QPoint newHead = m_snake.first() + QPoint(deltaX, deltaY);

    if (newHead.x() < 0 ||
        newHead.y() < 0 ||
        newHead.x() >= BoardSize ||
        newHead.y() >= BoardSize)
    {
        setMessage("You went out of bounds");
        mTimer.stop();
        return;
    }

    QPoint oldTail = m_snake.last();


    for (int i = m_snake.size() - 1; i > 0; --i)
    {
        m_snake[i] = m_snake[i - 1];
    }

    m_snake[0] = newHead;

    if (std::find(m_snake.begin() + 1,
                  m_snake.end(),
                  newHead) != m_snake.end())
    {
        setMessage("You lost");
        mTimer.stop();
        return;
    }

    if (newHead == mFood)
    {
        addSnakeSegment(oldTail);
        setFood(generateFoodPosition());
    }

    emit dataChanged(
        index(0, 0),
        index(m_snake.size() - 1, 0),
        {PositionRole});
}


void SnakeManager::changeDirection(int key)
{
    switch (key)
    {
    case Qt::Key_Left:
        if (mDirection != Direction::Right)
            mPendingDirection = Direction::Left;
        break;


    case Qt::Key_Right:
        if (mDirection != Direction::Left)
            mPendingDirection = Direction::Right;
        break;


    case Qt::Key_Up:
        if (mDirection != Direction::Down)
            mPendingDirection = Direction::Up;
        break;


    case Qt::Key_Down:
        if (mDirection != Direction::Up)
            mPendingDirection = Direction::Down;
        break;
    }
}


void SnakeManager::reset()
{
    beginResetModel();

    m_snake.clear();

    m_snake.append(QPoint(200, 300));

    mDirection = Direction::Right;
    mPendingDirection = Direction::Right;

    endResetModel();


    setFood(generateFoodPosition());

    mTimer.start();
}


void SnakeManager::clearSnake()
{
    beginResetModel();

    m_snake.clear();

    endResetModel();
}


void SnakeManager::addSnakeSegment(const QPoint &position)
{
    int newIndex = m_snake.size();

    beginInsertRows(
        QModelIndex(),
        newIndex,
        newIndex);

    m_snake.append(position);

    endInsertRows();
}


QPoint SnakeManager::generateFoodPosition() const
{
    while (true)
    {
        QPoint position(
            getRandomNumber(0, BoardCells - 1) * CellSize,
            getRandomNumber(0, BoardCells - 1) * CellSize);


        if (!isOccupied(position))
            return position;
    }
}


bool SnakeManager::isOccupied(const QPoint &position) const
{
    return std::find(
               m_snake.begin(),
               m_snake.end(),
               position) != m_snake.end();
}


QString SnakeManager::message() const
{
    return mMessage;
}


void SnakeManager::setMessage(const QString &message)
{
    if (mMessage == message)
        return;

    mMessage = message;
    emit messageChanged();
}


QPoint SnakeManager::food() const
{
    return mFood;
}


void SnakeManager::setFood(const QPoint &food)
{
    if (mFood == food)
        return;

    mFood = food;
    emit foodChanged();
}


void SnakeManager::requestQuit()
{
    QCoreApplication::quit();
}
