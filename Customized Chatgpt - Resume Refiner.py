import openai
import gradio as gr
import PyPDF2
import docx
from docx import Document


openai.api_key = "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"

messages = []

default_text_1 = "Hello, I will share a candidate's resume and a job description with you...\n" \
                 "1: Provide a summary of the candidate's resume.\n" \
                 "2: List the strengths and weaknesses of this candidate.\n" \
                 "3: Rate the candidate's resume in alignment with the job description."

default_text_2 = "Based on the job description, could you please enhance the resume...\n" \
                 "1: Remove any contact information and correct any typos.\n" \
                 "2: Incorporate project descriptions or technical details into the resume.\n" \
                 "3: Adjust the wording to enhance alignment with the job description."


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


def handle_file_upload(uploaded_file):
    if uploaded_file is None:
        return ""
    
    # checking the file suffix
    file_type = uploaded_file.name.split('.')[-1].lower()
    print("File Type:", file_type)
    
    if file_type == 'pdf':
        return extract_text_from_pdf(uploaded_file)
    elif file_type in ['docx', 'doc']:
        return extract_text_from_docx(uploaded_file)
    else:
        return "Unsupported file format, please try again"
    

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

    # Save the generated response as a Word document
    doc = docx.Document()
    doc.add_paragraph(ChatGPT_reply)
    doc.save("generated_response.docx")
    
    print("ChatGPT Reply:", ChatGPT_reply)

    return ChatGPT_reply, "generated_response.docx"


def main():
    demo = gr.Interface(
        fn=CustomChatGPT,
        inputs=["text", gr.File(label="Upload Resume")],
        outputs=["text", "file"],
        live=False,
        examples=[[default_text_1, None], [default_text_2, None]]
    )
    demo.launch(share=True)


if __name__ == "__main__":
    main()











# from docx.shared import Pt
# from docx.enum.text import WD_ALIGN_PARAGRAPH

# def save_as_resume_format(text):
#     doc = docx.Document()

#     # Set up some styles
#     style = doc.styles['Normal']
#     font = style.font
#     font.name = 'Arial'
#     font.size = Pt(12)

#     # Add text to the document with formatting
#     for line in text.split('\n'):
#         paragraph = doc.add_paragraph(line)

#         # Example: Apply bold to lines that end with a colon (like headings)
#         if line.endswith(':'):
#             paragraph.bold = True

#         # Example: Align center for specific lines
#         if 'Contact Information' in line:
#             paragraph.alignment = WD_ALIGN_PARAGRAPH.CENTER

#     # Save the document
#     doc.save("generated_resume.docx")

# def CustomChatGPT(user_input, uploaded_file):
#     global messages
#     resume_text = handle_file_upload(uploaded_file)

#     if resume_text:
#         messages.append({"role": "system", "content": resume_text})

#     messages.append({"role": "user", "content": user_input})
#     response = openai.ChatCompletion.create(
#         model="gpt-3.5-turbo",
#         messages=messages
#     )
#     ChatGPT_reply = response["choices"][0]["message"]["content"]
#     messages.append({"role": "assistant", "content": ChatGPT_reply})

#     # Save the generated response in resume format
#     save_as_resume_format(ChatGPT_reply)

#     return ChatGPT_reply, "generated_resume.docx"

# # ... rest of your code ...

