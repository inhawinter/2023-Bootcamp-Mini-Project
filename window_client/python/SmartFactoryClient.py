import requests

web_server_url = "http://192.168.15.245:80"

response = requests.get(web_server_url)
data = response.text

lines = data.split("\n")

ip_address = ''
if len(lines) >= 4:
    ip_address = lines[2].split(": ")[1]

print(f"IP address: {ip_address}")
print(lines[3])
