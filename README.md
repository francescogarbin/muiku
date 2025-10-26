# Maiku
LLM-based haiku generator for Gnome. Muiku generates poetry and displays it on a minimal UI.

Available as TUI (terminal user interface) perfect for Hyprland and GUI (graphical user interface) based on GTK-4.  

<p align="center">
<img width="752" height="460" alt="immagine" src="https://github.com/user-attachments/assets/c46efb9f-58db-4eda-9ab2-fecdded4d058" />
<p/>
	
## Maiku deploys an AI agent to consume verses from Llama 3.1 hosted as local model on your machines

The Maiku application adopts Langchain pointing it to Llama 3.1 running locally. In my case, Llama 3.1 is hosted on a ProxMox LXC container called Beskar, phisical host name is Mandalore, running on MyAss, a 4 machines cluster (My Assigned Server Space, hence MyAss).

### General notes for using Langchain in a client-server scenario.

Install the Langchain package:

```
pip install -U langchain
pip install -qU "langchain[llama3.1]"
```

Connect to model:

```
ollama_llm = Ollama( model="llama3.1", base_url="http://<server>:<port>" )
```

From then on:

```
from langchain.agents import create_agent

def get_weather(city: str) -> str:
    """Get weather for a given city."""
    return f"It's always sunny in {city}!"

	agent = create_agent(
    model="Ollama:llama3.1",
    tools=[get_weather],
    system_prompt="You are a cyberpunk Haiku poet",)

agent.invoke(
    {"messages": [{"role": "user", "content": "Write a Haiku about RAM getting full"}]})
```
