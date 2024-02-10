import pandas as pd
import dash
from dash import dcc, html
from dash.dependencies import Input, Output
import plotly.graph_objs as go

import io
import base64


# Read the CSV file and preprocess
df = pd.read_csv('JudeBellingham-modified.csv')
df = df.dropna()

#df = df.drop(['PK', 'PKatt', 'CrdY', 'CrdR'], axis=1)

seasonal_averages = df.select_dtypes(include='number').mean()

# Create the Dash application
app = dash.Dash(__name__)


# Create dropdown options
dropdowns = []
for col in df.columns:
    if df[col].dtype != 'float64' and df[col].dtype != 'int64':
        options = [{'label': 'All', 'value': 'All'}] + [{'label': str(val), 'value': val} for val in df[col].unique()]
        dropdowns.append(html.Div([
        html.Label(f'Select {col}:', style={'color': '#9e8081'}),  # Label color
        dcc.Dropdown(
            id=f'dropdown-{col}',
            options=options,
            value='All',
            style={
                'width': '99%',  # Width of the dropdown
                'display': 'inline-block',
                'marginRight': '10%',
                'borderRadius': '4px',
                'color': '#28a745',  # Text color
                'backgroundColor': '#d1e6cf',  # Background color
                'borderColor': '#007bff'  # Border color
            }
        )
])  )




fig = go.Figure()

# Initial plot with default data (using seasonal_averages as an example)
for col in seasonal_averages.index:
    fig.add_trace(go.Bar(
    x=[col], 
    y=[seasonal_averages[col]], 
    name=f'Seasonal Average - {col}', 
    marker_color='#276ccc',  # Corrected color string
    offset=-0.2, 
    width=0.4, 
    marker_line_width=2, 
    marker_line_color='#0a0a0a', 
    showlegend=False
))


fig.update_layout(
    title='Selected Mean VS All Games Average', 
    xaxis_title='Categories', 
    yaxis_title='Value', 
    barmode='overlay', 
    plot_bgcolor='rgba(100,20,250,0)', 
    paper_bgcolor='rgba(124,142,149,0.8)', 
    font=dict(family='Helvetica, Arial, sans-serif', size=12, color='black'), 
    margin=dict(l=150, r=50, t=50, b=50), 
    xaxis=dict(showgrid=True, zeroline=False, showline=False, showticklabels=True), 
    yaxis=dict(showgrid=False, zeroline=False, showline=False, showticklabels=True)
)



dcc.Graph(
    id='bar-plot',
    figure=fig,
    style={'margin': 'auto', 'width': '90%', 'height': '500px', 'border': '1px solid black'},
    config={
        'displayModeBar': True,  # True by default, set to False to hide the modebar
        'scrollZoom': True,  # Allows scrolling to zoom
        'responsive': True  # Make plot responsive to window resizing
    },
    animate=True,
    className='my-graph-class'
)

    


# Define the layout of the web application
app.layout = html.Div([
    # Create a title with a specific style
    html.H1('Individual Statistics Visualization', 
            style={'textAlign': 'center', 
                   'color': '#9e8081', 
                   'fontFamily': 'Arial, sans-serif', 
                   'fontSize': '24px', 
                   'fontWeight': 'bold'}),  # Adjust font size and weight as needed

    html.Div(
        dropdowns, 
        style={'display': 'flex', 'justifyContent': 'center', 'flexWrap': 'wrap', 'shapeRendering': 'crispEdges', 'marginTop': '20px', 'marginBottom': '20px', 'borderRadius': '15px'}
    ),
    html.Div(
        html.Button('Submit', id='submit-button', n_clicks=0, 
                    style={'backgroundColor': '#74ab7e', 
                           'color': 'white', 
                           'fontSize': '17px', 
                           'borderRadius': '10px', 
                           'padding': '8px'}),
        style={'textAlign': 'center', 'height': '75px', 'marginTop': '20px'}  # Centered button
    ),
    dcc.Graph(id='bar-plot', figure=fig, style={'margin': 'auto', 'width': '100%'})
])



# Define the callback function for the button click
@app.callback(
    Output('bar-plot', 'figure'),
    [Input('submit-button', 'n_clicks')],
    [Input(f'dropdown-{col}', 'value') for col in df.columns if df[col].dtype != 'float64' and df[col].dtype != 'int64']
)


def update_bar_plot(n_clicks, *args):
    if n_clicks == 0:
        return go.Figure()

    filters = {col: val for col, val in zip(df.columns, args) if val != 'All'}
    filtered_df = df
    for col, val in filters.items():
        filtered_df = filtered_df[filtered_df[col] == val]

    # Calculate the mean for numerical columns
    mean_values = filtered_df.select_dtypes(include='number').mean().round(2)

    # Create the bar plot
    fig = go.Figure()
    legend_added = set()  # Create a set to keep track of the legend items
    for i, col in enumerate(mean_values.index):


        selected_mean_name = f'Selected Mean - {col}'
        seasonal_average_name = f'All Game Average - {col}'

        mean_values[col].round(2)
        seasonal_averages[col].round(2)

        if 'Selected Mean' not in legend_added:
            fig.add_trace(go.Bar(x=[col], y=[mean_values[col]], name=selected_mean_name, marker_color='#276ccc', offset=-0.2, width=0.4, marker_line_width=2, marker_line_color='DarkSlateGrey'))
            legend_added.add(selected_mean_name)  # Add the item to the set

        if 'Seasonal Average' not in legend_added:
            fig.add_trace(go.Bar(x=[col], y=[seasonal_averages[col]], name=seasonal_average_name, marker_color='#d95293', offset=0.1, width=0.4, marker_line_width=2, marker_line_color='DarkSlateGrey'))
            legend_added.add(seasonal_average_name)  # Add the item to the set

        # Customize the bar shape and width
        # fig.add_trace(go.Bar(x=[col], y=[mean_values[col]], name='Selected Mean', marker_color='blue', offset=-0.2, width=0.4, marker_line_width=2, marker_line_color='DarkSlateGrey'))
        # fig.add_trace(go.Bar(x=[col], y=[seasonal_averages[col]], name='All Game Average', marker_color='red', offset=0.1, width=0.4, marker_line_width=2, marker_line_color='DarkSlateGrey'))


    
    # Customize the legend to include only "Selected Mean" and "Seasonal Average"
    fig.update_layout(
        legend=dict(
            x=1.1,  # Adjust the x position of the legend
            y=1.1,  # Adjust the y position of the legend
            traceorder='normal',  # Arrange legend items in normal order
            itemsizing='constant',  # Keep legend item sizes constant
            bgcolor='rgba(255, 255, 255, 0.5)',  # Legend background color with transparency
            bordercolor='rgba(0, 0, 0, 0.5)',  # Legend border color with transparency
            borderwidth=2,  # Legend border width
            font=dict(family='Arial, sans-serif', size=12, color='black')  # Legend font customization
        )
    )


    fig.update_layout(title='Selected Mean VS All Games Average', xaxis_title='Categories', yaxis_title='Value', barmode='overlay', plot_bgcolor='rgba(100,20,250,0)',paper_bgcolor='rgba(124,142,149,0.8)', font=dict(family='Helvetica, Helvetica', size=12, color='black'), margin=dict(l=150, r=50, t=50, b=50), xaxis=dict(showgrid=True, zeroline=False, showline=False, showticklabels=True), yaxis=dict(showgrid=False, zeroline=False, showline=False, showticklabels=True))

    return fig


# Run the application
if __name__ == '__main__':
    app.run_server(debug=True, port=8099)

