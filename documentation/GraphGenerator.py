import igraph  # To install: `pip3 install python-igraph`

import os


pure_subtrees = {
    'code/lexer',
    'code/parser',
    'code/parser/Parser.cpp'
}

uses_global = {

}


pure = '#00FF00'
subtree_pure = '#00BFFF'

def createGraph(graph, parent, directory, purity=subtree_pure):
    for root, directories, files in os.walk(directory, topdown=True):
        graph.add_vertex(name=directory)

        if directory in pure_subtrees:
            purity = subtree_pure
            graph.vs[len(graph.vs) - 1]['vertex_color'] = pure
        else:
            graph.vs[len(graph.vs) - 1]['vertex_color'] = purity

        graph.add_edge(source=parent, target=len(graph.vs) - 1)
        root_node = len(graph.vs) - 1
        for file in files:
            if file.startswith('test_'):
                continue
            graph.add_vertex(name=file)
            if os.path.join(directory, file) in pure_subtrees:
                graph.vs[len(graph.vs) - 1]['vertex_color'] = pure
            else:
                graph.vs[len(graph.vs) - 1]['vertex_color'] = purity

            graph.add_edge(source=root_node, target=len(graph.vs) - 1)
        for directory_ in directories:
            createGraph(graph=graph, parent=root_node, directory=os.path.join(root, directory_), purity=purity)
        return


if __name__ == '__main__':
    graph = igraph.Graph()
    graph.to_directed()
    graph.add_vertex(name='Unnamed-Language')

    for root, directories, files in os.walk('code', topdown=True):
        graph.add_vertex(name=root)
        root_node = len(graph.vs) - 1
        for file in files:
            if file.startswith('test_'):
                continue
            graph.add_vertex(name=file)
            graph.add_edge(source=root_node, target=len(graph.vs) - 1)
        for directory in directories:
            createGraph(graph=graph, parent=root_node, directory=os.path.join(root, directory))
        break

    graph.add_edge(source=0, target=1)


    layout = graph.layout('kk')
    style = {}

    style['vertex_size'] = 200

    style['vertex_color'] = ['red' for vertex in graph.vs]
    style['vertex_color'] = [vertex['vertex_color'] if vertex['vertex_color'] else 'red' for vertex in graph.vs]
    style['vertex_label'] = graph.vs['name']
    style['edge_curved'] = 0.3
    style['layout'] = layout
    style['bbox'] = (5000, 5000)
    style['margin'] = 300
    igraph.plot(
        graph,
        'documentation/dependency_graph.png', **style)
























    # draw_graph(name='project', root, graph_size, type_of_graph, excluded_nodes, highlight)