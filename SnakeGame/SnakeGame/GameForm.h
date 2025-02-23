#pragma once

// ����������, ����������� ������� time() � srand()
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

		// obj - ������
		// e - �������� �� ��������� ������ ����
		virtual void OnPaint(System::Object^ obj, PaintEventArgs^ e) override {
			// ���������� � ����������� ���������� ��� ����������������� (�������������)
			Graphics^ g = e->Graphics;

			// fruitBrush - ��������� ������
			Brush^ fruitBrush = gcnew SolidBrush(Color::Red);
			// ����������� ������� ������������ �����
			g->FillRectangle(fruitBrush, fruitPosition.X, fruitPosition.Y, blockSize, blockSize);

			// Brush - �����, shaneBrush - ��������� ����
			// ������ ������ (SolidBrush) ������ ���� ���������� �����
			Brush^ snakeBrush = gcnew SolidBrush(Color::GreenYellow);
			// ������ ������������ ��� ������ - ����
			// ������ �������� - �����
			// ������ � ������ ��������� - ���������� �� X � Y
			// �������� � ����� ��������� - ������ ���� �� ������ � ������

			//���������� ���� ������ � ������������ ������ ������
			for each (Point element in snake) {
				// ��������� ������� �������� ��������
				g->FillRectangle(snakeBrush, element.X, element.Y, blockSize, blockSize);
			}
			
		}

	private:
		System::ComponentModel::Container ^components;
		// ������� ����
		List<Point>^ snake;
		// ������� ������
		Point fruitPosition;
		// ����������, �������� ������ ������, �� ������� ����� �������� �������
		const int blockSize = 20;

		// ������ ��� �������� ����������� ��������
		Timer^ timer;
		// ����������, ���������� �� �����������
		int moveX = 1;
		int moveY = 0;

		// ���������� ��������� �������
		int fruitCount = 0;
		// ��������� �������
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

			// ������ ��������� �������
			scoreLabel = gcnew Label();
			// ������������� ���� ������
			scoreLabel->ForeColor = Color::White;
			// ������������� ���� ������� ���� - ��� ����������
			scoreLabel->BackColor = Color::Transparent;
			// ������������� �����
			scoreLabel->Text = "Score: 0";
			// ��������� ������������ �������
			scoreLabel->Location = Point(10, 10);
			// ��������� ��������� ������� �� �����
			this->Controls->Add(scoreLabel);

			// ��������� ������ � ������ (��� ����� ���������� ������ � ������ ����� �������)
			// �������� ������
			snake = gcnew List<Point>();
			// ��������� ������� ������ 
			snake->Add(Point(100, 100));
			

			// ����������, ����� ������ ����������� � ��������� �����������
			srand(time(NULL));
			PlaceFruit();

			// ������ ���� ������
			timer = gcnew Timer();
			// ��������� �������� ������ (� ��)
			timer->Interval = 200;
			// ����� ������ ���������� ������� ����� ���������� �������
			// ������ �������� - ����������
			// ������ �������� - �������, ������� ����� ����������
			timer->Tick += gcnew EventHandler(this, &GameForm::OnTimerTick);
			// ��������� ������
			timer->Start();

			// �������� ������ � ��������� ��������� ��� ���������
			// ������ �������� - �������� (����), � ������� ��������
			// ������ �������� - �������, � ������� ��������
			this->Paint += gcnew PaintEventHandler(this, &GameForm::OnPaint);
			// ������������ ������� ������
			this->KeyDown += gcnew KeyEventHandler(this, &GameForm::OnKeyDown);
		}
#pragma endregion
		// ��������� �������, ������� ����� �������� �� ���������� ������
		void PlaceFruit() {
			// ������� ������������ ������ � ������ ����������, � ����� ��� ��������� �������� � ������� �������
			int maxX = ClientSize.Width / blockSize;	// ���������� �����������, ������� ����� ��������� �� ������
			int maxY = ClientSize.Height / blockSize;	// ���������� �����������, ������� ����� ��������� �� ������

			// ���� �������� �� ��� ���, ���� ������� ������ ����� ��������� � �����-���� �� ��������� ������
			do {
				// ������ �������� ��������� ����� �� 0 �� maxX
			// �������� �� blockSize, ����� �� ����� ������� ������� ��������� � �������
				fruitPosition = Point(rand() % maxX * blockSize, rand() % maxY * blockSize);
			} while (snake->Contains(fruitPosition));
		}

		void OnTimerTick(Object^ obj, EventArgs^ e) {
			// �������� ������� ��� ����������� ����
			MoveSnake();

			// �������� �� ������������ ��������������� � ����� �� ������ ����
			// �������������� � ����� ����� ���� ��� ������� ��������,
			// ���� �������������� � ������ ����� ���� ��� ������ ��������
			if (snake[0].X < 0 || snake[0].Y < 0 || snake[0].X > this->ClientSize.Width || snake[0].Y > this->ClientSize.Height) {
				// ������������� ������
				timer->Stop();
				// ������������� ������� ����������� ������������
				MessageBox::Show("Game Over");
				// ��������� ����� ���������� ��������
				return;
			}

			//  ���� ��� ����� ������ ������ ��� ����� 4, ����� ����� ������ �������� �� ��,
			// ����� �� ������ ���� ��� �� ���
			if (snake->Count >= 4) {
				// ���� ������� ������ ���� ��������� � �������� ������ � �����, �� ��� ��� ����
				for (int i = 1; i < snake->Count; i++) {
					if (snake[0] == snake[i]) {
						// ������������� ������
						timer->Stop();
						// ������������� ������� ����������� ������������
						MessageBox::Show("Game Over: You ate yourself");
						// ��������� ����� ���������� ��������
						return;
					}
				}
			}

			// ���������, ��������� �� ������� ���� � ������
			if (snake[0] == fruitPosition) {
				// ����������� ���������� ��������� �������
				fruitCount++;
				// ��������� ����� � ������� scoreLabel
				scoreLabel->Text = "Score: " + fruitCount.ToString();

				// ����������� ����
				GrowthSnake();
				// ������ ����� �����
				PlaceFruit();
			}

			// ����� ������������� ���������� ������� OnPaint
			this->Invalidate();
		}

		void MoveSnake() {
			// ������������� ������ ������� ������
			Point newHead = snake[0];

			// ������������� ����� ����������
			newHead.X += moveX * blockSize;
			newHead.Y += moveY * blockSize;

			// ����� �� �������������� ��� ����, � �������� ������ ��������
			// ����� ����������� newHead
			snake->Insert(0, newHead);
			// ������� ��������� ������� (�� �������: ������ ������� - 1)
			snake->RemoveAt(snake->Count - 1);
		}

		void GrowthSnake() {
			// ������ ����� ������ � ��������� ��� � ������
			Point newHead = snake[0];

			newHead.X += moveX * blockSize;
			newHead.Y += moveY * blockSize;

			// � �������� ����� ������ ��������� ����� �������
			snake->Insert(0, newHead);
		}

		void OnKeyDown(Object^ obj, KeyEventArgs^ e) {
			// ��������� ��� ������� �������
			switch (e->KeyCode)
			{
			// ��������� �����
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
