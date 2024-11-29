namespace Calculadora
{
    public partial class Form1 : Form
    {
        double number1 = 0, number2 = 0; //declaracion de variables numeros
        char actualOperation = '\0'; //operador actual
        char oldOperation = '\0'; //operador anterior
        public Form1()
        {
            InitializeComponent();
        }

        private void addNumber(object sender, EventArgs e) //funcion botones numeros
        {
            var button = ((Button)sender);
            if (resultBox.Text == "0" || resultBox.Text == "Err")
                resultBox.Text = "";
            resultBox.Text += button.Text;
        }

        private void operatorButton(object sender, EventArgs e) //funcion botones operadores
        {
            var button = ((Button)sender);
            number1 = Convert.ToDouble(resultBox.Text);
            actualOperation = Convert.ToChar(button.Tag);
            oldOperation = '\0';

            if (actualOperation == '√')
            {
                number1 = Math.Sqrt(number1); //raiz cuadrada
                resultBox.Text = Convert.ToString(number1);
            }
            else if (actualOperation == '²')
            {
                number1 = Math.Pow(number1, 2); //elevar al cuadrado
                resultBox.Text = Convert.ToString(number1);
            }
            else if (actualOperation == 'i')
            {
                number1 = (1 / number1); //inverso del numero
                resultBox.Text = Convert.ToString(number1);
            }
            else
                resultBox.Text = "0";
        }

        private void resultButton(object sender, EventArgs e)
        {
            if (actualOperation != oldOperation) //si es diferente 
            {
                number2 = Convert.ToDouble(resultBox.Text); //usa el numero actual para realizar la operacion
            }

            if (actualOperation == '+')
            {
                resultBox.Text = (number1 + number2).ToString();
            }
            else if (actualOperation == '-')
            {
                resultBox.Text = (number1 - number2).ToString();
            }
            else if (actualOperation == '×')
            {
                resultBox.Text = (number1 * number2).ToString();
            }
            else if (actualOperation == '÷')
            {
                if (number2 != 0)
                {
                    resultBox.Text = (number1 / number2).ToString();
                }
                resultBox.Text = "Err";
            }
            else if (actualOperation == '%')
            {
                resultBox.Text = ((number1 / 100) * number2).ToString();
            }
            oldOperation = actualOperation; //guarda la operacion actual en la anterior
            if (resultBox.Text != "Err")
            {
                number1 = Convert.ToDouble(resultBox.Text); //guarda el resultado en numero 1

            }
        }

        private void Cbutton(object sender, EventArgs e) //boton limpiar todo
        {
            number1 = 0;
            number2 = 0;
            resultBox.Text = "0";
            actualOperation = '\0';
            oldOperation = '\0';
        }

        private void CEbutton(object sender, EventArgs e)
        {
            resultBox.Text = "0";
        }

        private void backspaceButton(object sender, EventArgs e)
        {
            if (resultBox.Text.Length > 1)
                resultBox.Text = resultBox.Text.Substring(0, resultBox.Text.Length - 1);
            else resultBox.Text = "0";
        }

        private void dotButton(object sender, EventArgs e)
        {
            if (!resultBox.Text.Contains("."))
            {
                resultBox.Text += ".";
            }
        }

        private void plusMinus(object sender, EventArgs e)
        {
            double tempNumber = Convert.ToDouble(resultBox.Text);
            if (!resultBox.Text.Contains("0"))
            {
               resultBox.Text = (tempNumber * (-1)).ToString(); 
            }
        }
    }
}