#pragma once

// Библиотеки, содержищаие функции time() и srand()
#include<stdlib.h>
#include <time.h>

namespace SnakeGame {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;
	using namespace System::Collections::Generic;

	public ref class GameForm : public System::Windows::Forms::Form
	{
	public:
		GameForm(void)
		{
			InitializeComponent();
		}

	protected:
		~GameForm()
		{
			if (components)
			{
				delete components;
			}
		}

		// obj - объект
		// e - отвечает за рисование внутри окна
		virtual void OnPaint(System::Object^ obj, PaintEventArgs^ e) override {
			// Обращается к графическим настройкам для совершенствования (дорисовывания)
			Graphics^ g = e->Graphics;

			// fruitBrush - отрисовка фрукта
			Brush^ fruitBrush = gcnew SolidBrush(Color::Red);
			// Аналогичным образом отрисовываем фрукт
			g->FillRectangle(fruitBrush, fruitPosition.X, fruitPosition.Y, blockSize, blockSize);

			// Brush - кисть, shaneBrush - отрисовка змеи
			// Твёрдой линией (SolidBrush) рисуем змею указанного цвета
			Brush^ snakeBrush = gcnew SolidBrush(Color::GreenYellow);
			// Теперь отрисовываем сам объект - змею
			// Первый параметр - кисть
			// Второй и третий параметры - координаты по X и Y
			// Четвёртый и пятый параметры - размер змеи по ширине и высоте

			//Перебираем весь список и отрисовываем каждый объект
			for each (Point element in snake) {
				// Указываем позицию текущего элемента
				g->FillRectangle(snakeBrush, element.X, element.Y, blockSize, blockSize);
			}
			
		}

	private:
		System::ComponentModel::Container ^components;
		// Позиция змеи
		List<Point>^ snake;
		// Позиция фрукта
		Point fruitPosition;
		// Переменаня, хранящая размер блоков, из которых будут состоять объекты
		const int blockSize = 20;

		// Таймер для создания цикличности действий
		Timer^ timer;
		// Переменные, отвечающие за перемещение
		int moveX = 1;
		int moveY = 0;

		// Количество съеденных фруктов
		int fruitCount = 0;
		// Текстовая надпись
		Label^ scoreLabel;


#pragma region Windows Form Designer generated code

		void InitializeComponent(void)
		{
			this->SuspendLayout();

			this->AutoScaleDimensions = System::Drawing::SizeF(8, 16);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(400, 400);
			this->Text = L"Snake Game";
			this->BackColor = Color::Black;
			this->ResumeLayout(false);

			// Создаём текстовую надпись
			scoreLabel = gcnew Label();
			// Устанавливаем цвет текста
			scoreLabel->ForeColor = Color::White;
			// Устанавливаем цвет заднего фона - как прозрачный
			scoreLabel->BackColor = Color::Transparent;
			// Устанавливаем текст
			scoreLabel->Text = "Score: 0";
			// Указываем расположение надписи
			scoreLabel->Location = Point(10, 10);
			// Добавляем текстовую надпись на экран
			this->Controls->Add(scoreLabel);

			// Размещаем змейку в центре (для этого координаты ширины и высоты делим пополам)
			// Выделяем память
			snake = gcnew List<Point>();
			// Добавляем элемент змейки 
			snake->Add(Point(100, 100));
			

			// Необходимо, чтобы фрукты размещались в случайных координатах
			srand(time(NULL));
			PlaceFruit();

			// Создаём этот объект
			timer = gcnew Timer();
			// Указываем интервал работы (в мс)
			timer->Interval = 200;
			// Через каждый промежуток времени будет вызываться функция
			// Первый параметр - приложение
			// Второй параметр - функция, которая будет вызываться
			timer->Tick += gcnew EventHandler(this, &GameForm::OnTimerTick);
			// Запускаем таймер
			timer->Start();

			// Выделяем память и указываем аргументы для отрисовки
			// Первый аргумент - контекст (окно), с которым работаем
			// Второй аргумент - функция, с которой работаем
			this->Paint += gcnew PaintEventHandler(this, &GameForm::OnPaint);
			// Отслеживание нажатой кнопки
			this->KeyDown += gcnew KeyEventHandler(this, &GameForm::OnKeyDown);
		}
#pragma endregion
		// Описываем функцию, которая будет отвечать за размещение фрукта
		void PlaceFruit() {
			// Получим максимальную ширину и высоту приложения, а далее эти параметры переведём в блочную систему
			int maxX = ClientSize.Width / blockSize;	// Количество квадратиков, которое можем поместить по ширине
			int maxY = ClientSize.Height / blockSize;	// Количество квадратиков, которое можем поместить по высоте

			// Цикл работает до тех пор, пока позиция фрукта будет совпадать с каким-либо из элементов змейки
			do {
				// Теперь получаем случайное число от 0 до maxX
			// Умножаем на blockSize, чтобы из нащей блочной системы вернуться в пиксели
				fruitPosition = Point(rand() % maxX * blockSize, rand() % maxY * blockSize);
			} while (snake->Contains(fruitPosition));
		}

		void OnTimerTick(Object^ obj, EventArgs^ e) {
			// Вызываем функцию для перемещения змеи
			MoveSnake();

			// Проверка на отслеживание соприкосновения с одной из сторон окна
			// Соприкоснулись с левым краем окна или верхней границей,
			// либо соприкоснулась с правым краем окна или нижней границей
			if (snake[0].X < 0 || snake[0].Y < 0 || snake[0].X > this->ClientSize.Width || snake[0].Y > this->ClientSize.Height) {
				// Останавливаем таймер
				timer->Stop();
				// Дополнительно выведем уведомление пользователю
				MessageBox::Show("Game Over");
				// Блокируем любые дальнейшие действия
				return;
			}

			//  Если вся длина змейки больше или равна 4, тогда будем делать проверку на то,
			// съела ли змейка себя или же нет
			if (snake->Count >= 4) {
				// Если позоция головы змеи совпадает с позицией другой её части, то она ест себя
				for (int i = 1; i < snake->Count; i++) {
					if (snake[0] == snake[i]) {
						// Останавливаем таймер
						timer->Stop();
						// Дополнительно выведем уведомление пользователю
						MessageBox::Show("Game Over: You ate yourself");
						// Блокируем любые дальнейшие действия
						return;
					}
				}
			}

			// Проверяем, совпадают ли позиции змеи и фрукта
			if (snake[0] == fruitPosition) {
				// Увеличиваем количество съеденных фруктов
				fruitCount++;
				// Обновляем текст в объекте scoreLabel
				scoreLabel->Text = "Score: " + fruitCount.ToString();

				// Увеличиваем змею
				GrowthSnake();
				// Создаём новый фрукт
				PlaceFruit();
			}

			// Будет автоматически вызываться функция OnPaint
			this->Invalidate();
		}

		void MoveSnake() {
			// Устанавливаем первый элемент списка
			Point newHead = snake[0];

			// Устанавливаем новые координаты
			newHead.X += moveX * blockSize;
			newHead.Y += moveY * blockSize;

			// Чтобы не перерисовывать всю змею, в качестве нового элемента
			// будем подставлять newHead
			snake->Insert(0, newHead);
			// Удаляем последний элемент (по индексу: размер массива - 1)
			snake->RemoveAt(snake->Count - 1);
		}

		void GrowthSnake() {
			// Создаём новый объект и добавляем его в список
			Point newHead = snake[0];

			newHead.X += moveX * blockSize;
			newHead.Y += moveY * blockSize;

			// В качестве новой головы добавляем новый элемент
			snake->Insert(0, newHead);
		}

		void OnKeyDown(Object^ obj, KeyEventArgs^ e) {
			// Проверяем код нажатой клавиши
			switch (e->KeyCode)
			{
			// Стрелочка вверх
			case Keys::Up:
				moveX = 0;
				moveY = -1;
				break;
			case Keys::Down:
				moveX = 0;
				moveY = 1;
				break;
			case Keys::Left:
				moveX = -1;
				moveY = 0;
				break;
			case Keys::Right:
				moveX = 1;
				moveY = 0;
				break;
			}
		}
	};
}
