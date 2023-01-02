#include <iostream>
#include <QObject>

class Counter : public QObject{
    Q_OBJECT
    private:
        int m_count;

    public:
        Counter(int startValue) : m_count{startValue} {}
        int count() const { return m_count; }

        int Increment() {
            emit incrementSignal();
            increment();
            return m_count;
        }

        int Decrement() {
            emit decrementSignal();
            decrement();
            return m_count;
        }

    signals:
        void incrementSignal();
        void decrementSignal();

    private slots:
        void increment() {
            ++m_count;
        };

        void decrement() {
            --m_count;
        };
};

int main() {
    Counter c1(10), c2(0);

    QObject::connect(&c1, SIGNAL(incrementSignal()),
                     &c2, SLOT(increment()));

    QObject::connect(&c2, SIGNAL(incrementSignal()),
                     &c1, SLOT(increment()));

    QObject::connect(&c1, SIGNAL(decrementSignal()),
                     &c2, SLOT(decrement()));

    QObject::connect(&c2, SIGNAL(decrementSignal()),
                     &c1, SLOT(decrement()));

    std::cout << "original state:\n";
    std::cout << "c1[" << c1.count() << "]\tc2[" << c2.count() << "]\n\n";

    c1.Increment();
    std::cout << "after incrementing c1:\n";
    std::cout << "c1[" << c1.count() << "]\tc2[" << c2.count() << "]\n\n";

    c2.Increment();
    std::cout << "after incrementing c2:\n";
    std::cout << "c1[" << c1.count() << "]\tc2[" << c2.count() << "]\n\n";

    c1.Decrement();
    std::cout << "after decrementing c1:\n";
    std::cout << "c1[" << c1.count() << "]\tc2[" << c2.count() << "]\n\n";

    c2.Decrement();
    std::cout << "after decrementing c2:\n";
    std::cout << "c1[" << c1.count() << "]\tc2[" << c2.count() << "]\n\n";
}

#include "main.moc"
