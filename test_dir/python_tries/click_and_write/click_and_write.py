import pyautogui
import time

def click_and_write(x, y, text, delay=0.1):
    # """
    # Moves the mouse to the specified (x, y) coordinates, clicks, and types the given text.

    # Parameters:
    # x (int): The x-coordinate on the screen.
    # y (int): The y-coordinate on the screen.
    # text (str): The text to be typed after clicking.
    # delay (float): Delay in seconds before typing the text after clicking.
    # """
    #print mouce position
    time.sleep(2)
    print(pyautogui.position())

    # Move the mouse to the specified coordinates
    # pyautogui.moveTo(x, y)
    
    # # Click at the specified coordinates
    # pyautogui.click()
    
    # # Wait for the specified delay
    # time.sleep(delay)
    
    # # Type the given text
    # pyautogui.write(text)   
    print(f"Clicked at ({x}, {y}) and typed: '{text}'")
# Example usage
if __name__ == "__main__":
    time.sleep(2)
    print(pyautogui.position())
    # click_and_write(500, 300, "Hello, World!", delay=0.5)   