import requests
import tkinter as tk
import urllib.request
from bs4 import BeautifulSoup


def update_labels():
    web_server_url = "http://192.168.15.245:80"

    response = requests.get(web_server_url)
    data = response.text

    lines = data.split("\n")

    ip_address = ''
    if len(lines) >= 4:
        ip_address = lines[2].split(": ")[1]

    lbl_ip.config(text=f"IP address: {ip_address}")
    lbl_temperature.config(text=lines[3])

    api = 'https://www.kma.go.kr/weather/forecast/mid-term-rss3.jsp'
    urls = urllib.request.urlopen(api).read()
    soup = BeautifulSoup(urls, "html.parser")
    #cities = soup.find_all("city")
    tmin = soup.find_all("tmn")  # lowest temperature
    tmax = soup.find_all("tmx")  # highest temperature
    yyyymmdd = soup.find_all("tmef")  # yyyy-mm-dd 00:00

    lbl_incheon_temperature.config(text=f'City : Incheon, Min temp. : {tmin[1 * 13].string}C, Max temp. : {tmax[1 * 13].string}C\nDates(Midterm forecast) : {yyyymmdd[1*13].string}')
    #print(f'{cities[1].string} Min temp. : {tmin[1 * 13].string}, Max temp. : {tmax[1 * 13].string}')


# create gui component
current_temperature_condition = tk.Tk()
current_temperature_condition.title("Inha Smart Factory!")
current_temperature_condition.geometry("400x200")
lbl_ip = tk.Label(current_temperature_condition, text="IP address: ")
lbl_temperature = tk.Label(current_temperature_condition, text="")
lbl_incheon_temperature = tk.Label(current_temperature_condition, text="city/min/max")
btn_check_temperature = tk.Button(current_temperature_condition, text="Check Temperature!", command=update_labels)

# layout
lbl_ip.grid(row=0, column=0)
lbl_temperature.grid(row=0, column=1)
lbl_incheon_temperature.grid(row=1, column=0, columnspan=2, sticky=tk.EW)
btn_check_temperature.grid(row=2, column=0, columnspan=2, sticky=tk.EW)

current_temperature_condition.mainloop()
