# developing in progress .......

import openai
import gradio as gr
import PyPDF2
import docx
from docx import Document
from gradio import CSVLogger
import os

openai.api_key = ""

messages = []

#pre-filled text examples
default_text_1 = "Hello, I will share a candidate's resume with you. Please provide a summary for me" 
                
default_text_2 = "Hello, I will share a candidate's resume with you. Please desensitize the resume by removing or redacting any personal contact information like the phone number, home address, and email address. Also remove or mask any private details like ID numbers, exact dates for education and work history, and compensation details. Double check that no other private information is included. The goal is to remove personally identifying details while still allowing this candidate's professional qualifications to be visible to potential employers."

default_text_3 = "Please verify this candidate's academic credential to your best knowledge by checking whether the school, major, and diploma are valid by searching online"

def set_text_1():
    return default_text_1
def set_text_2():
    return default_text_2
def set_text_3():
    return default_text_3

# extracting text from pdf
def extract_text_from_pdf(file):
    reader = PyPDF2.PdfReader(file)
    text = ""
    for page in reader.pages:
        text += page.extract_text()
    return text

# extracting text from doc
def extract_text_from_docx(file):

    doc = docx.Document(file)
    all_text = []

    for para in doc.paragraphs:
        # Extract the text from the current paragraph
        paragraph_text = para.text
        all_text.append(paragraph_text)

    combined_text = " ".join(all_text)
    return combined_text

def handle_file_upload(uploaded_files):
    combined_text = ""
    if uploaded_files is None:
        return None
    for uploaded_file in uploaded_files:
        file_type = uploaded_file.name.split('.')[-1].lower()
        if file_type == 'pdf':
            combined_text += extract_text_from_pdf(uploaded_file)
        elif file_type in ['docx', 'doc']:
            combined_text += extract_text_from_docx(uploaded_file)
        else:
            combined_text += "Unsupported file format, please upload a PDF or Word document.\n"
    return combined_text

    
def clear_inputs():
    return "", None

def log_conversation(input, reply):
    with open("log.csv", "a") as log_file:
        log_file.write(f"user_input: {input},\n\n Chatgpt: {reply}\n\n")


def CustomChatGPT(user_input, uploaded_file):
    global messages
    resume_text = handle_file_upload(uploaded_file)
    print("Resume text from the file:", resume_text)

    if resume_text:
        messages.append({"role": "system", "content": resume_text})

    messages.append({"role": "user", "content": user_input})
    response = openai.ChatCompletion.create(
        model="gpt-3.5-turbo",
        messages=messages
    )
    ChatGPT_reply = response["choices"][0]["message"]["content"]
    messages.append({"role": "assistant", "content": ChatGPT_reply})

    # Save to log
    log_conversation(user_input, ChatGPT_reply)
    


    # Save the generated response as a Word document
    doc = docx.Document()
    doc.add_paragraph(ChatGPT_reply)
    doc.save("generated_response.docx")
    
    print("ChatGPT Reply:", ChatGPT_reply)

    return ChatGPT_reply, "generated_response.docx"

def get_log_file():
    return "log.csv"
def read_log_file():
    if os.path.exists("log.csv"):
        with open("log.csv", "r") as file:
            return file.read()
    else:
        return "Log file is empty or doesn't exist."
def clear_log_file():
    if not os.path.exists("log.csv"):
        return "file not exist"
    else:
        with open("log.csv", "w") as log_file:
            log_file.write("") 
        return "Log cleared"


def main():
   # customized buttons
    css = """
    button.green-button {
        appearance: none;
        background-color: #2ea44f;
        border: 1px solid rgba(27, 31, 35, .15);
        border-radius: 6px;
        box-shadow: rgba(27, 31, 35, .1) 0 1px 0;
        box-sizing: border-box;
        color: #fff;
        cursor: pointer;
        display: inline-block;
        font-family: -apple-system,system-ui,"Segoe UI",Helvetica,Arial,sans-serif,"Apple Color Emoji","Segoe UI Emoji";
        font-size: 14px;
        font-weight: 600;
        line-height: 20px;
        padding: 6px 16px;
        position: relative;
        text-align: center;
        text-decoration: none;
        user-select: none;
        -webkit-user-select: none;
        touch-action: manipulation;
        vertical-align: middle;
        white-space: nowrap;
        }

    button.green-button:hover {
        background-color: #2c974b;
        }
    .red-button {
        background-color: #f44336;
        color: white;
        padding: 14px 20px;
        margin: 8px 0;
        border: none;
        border-radius: 4px;
        cursor: pointer;
    }
    .red-button:hover {
        background-color: #d32f2f;
    }
    .blue-button {
        background-color: #2196F3;
        color: white;
        padding: 14px 20px;
        margin: 8px 0;
        border: none;
        border-radius: 4px;
        cursor: pointer;
    }
    .blue-button:hover {
        background-color: #1976D2;
    }

    """
    # structuring the page with columns and rows
    with gr.Blocks(css=css) as demo:
        with gr.Row():
            with gr.Column():
                # Title and Description
                gr.Markdown("# Resume Analysis Tool")
                gr.Markdown("Select a template or enter your text:")

                # Template buttons and input fields
                
                text_input = gr.Textbox(elem_classes=["custom-text-input"])
                file_input = gr.File(file_count="multiple", label="Upload Resume", elem_classes=["custom-text-input"])

                btn1 = gr.Button("Summary", elem_classes=["blue-button"])
                btn2 = gr.Button("Desensitization", elem_classes=["blue-button"])
                btn3 = gr.Button("Academic Credentials", elem_classes=["blue-button"])

                submit_btn = gr.Button("Submit", elem_classes=["green-button"])
                gr.ClearButton(components=[text_input, file_input], elem_classes=["red-button"])

                # Update text input with template text
                btn1.click(fn=set_text_1, inputs=[], outputs=text_input)
                btn2.click(fn=set_text_2, inputs=[], outputs=text_input)
                btn3.click(fn=set_text_3, inputs=[], outputs=text_input)

            with gr.Column():
                # Output fields
                output_text, output_file = gr.Text(), gr.File()

                # Handle submit action
                submit_btn.click(fn=CustomChatGPT, inputs=[text_input, file_input], outputs=[output_text, output_file])

                log_text = gr.Textbox(label="Log Content", interactive=False)
                view_log_button = gr.Button("View Log", elem_classes=["green-button"])
                view_log_button.click(fn=read_log_file, inputs=[], outputs=log_text)

                # Buttons for downloading and clearing the log file
                download_log_button = gr.Button("Download Log File", elem_classes=["green-button"])
                clear_log_button = gr.Button("Clear Log File", elem_classes=["red-button"])
                log_file_output = gr.File(label="Log File")
                download_log_button.click(fn=get_log_file, inputs=[], outputs=log_file_output)
                clear_log_button.click(fn=clear_log_file, inputs=[], outputs=[])

    demo.launch(share=True)

if __name__ == "__main__":
    main()







