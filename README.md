# Tic-Tac-Toe game AI using the Minimax Algorithm

## Making the unbeatable Tic-Tac-Toe game yourself
1. You can reference the tinkercad design [here](https://www.tinkercad.com/things/ewSBIPnRqvL-ai-tic-tac-toe) to wire up the circuit.

2. For the code, you can paste the code in `arduino.ino` into the Arduino IDE. Then drag in the `move_table.h` file into the same folder as the `arduino.ino` file.

3. Then plug in your Arduino board and upload the code!

## Running the python code
### Prerequisites
- [pyenv](https://github.com/pyenv/pyenv) for Python version management
- [pip](https://pip.pypa.io/) (comes with Python)
- [uv](https://docs.astral.sh/uv/) for package management

### Installation

1. **Clone the repository:**
   ```bash
   git clone <repository-url>
   ```

2. **Set up Python version with pyenv:**
   ```bash
   # pyenv will automatically use the version specified in .python-version
   pyenv install 3.11.4  # if not already installed
   pyenv local 3.11.4    # sets the local Python version
   ```

3. **Create and activate a virtual environment:**
   ```bash
   python -m venv .venv
   source .venv/bin/activate  # On Windows: .venv\Scripts\activate
   ```

4. **Install the package in development mode:**
   ```bash
   pip install -e .
   ```

## Updating
You can add packages to your project by running:
```bash
uv add <package>
```