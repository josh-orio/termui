# termui — Class Relations

This file shows the structural layout of the termui library and how data is passed between the layers of abstraction.

```
┌───────────────────────────────────────────┐   
│  ┌────────┐         ┌───────────┐         │   
│  │  Base  │         │  Display  │         │◄─┐
│  └───┬────┘         └─────┬─────┘         │  │
└──────┼────────────────────┼───────────────┘  │
       │                    │                  │
┌──────┼────────────────────┼───────────────┐  │
│      │  ┌───────────┐     │               │  │
│      ├─►│  Widgets  │◄────┤               │◄─┤
│      │  └───────────┘     │               │  │
└──────┼────────────────────┼───────────────┘  │
       │                    │                  │
┌──────┼────────────────────┼───────────────┐  │
│      │  ┌──────────────┐  │               │  │
│      └─►│  Interfaces  │◄─┘               │◄─┤
│         └──────┬───────┘                  │  │
└────────────────┼──────────────────────────┘  │
                 │                             │
┌────────────────▼──────────────────────────┐  │
│         ┌────────────┐    ┌────────────┐  │  │
│         │  Terminal  ├───►│  Renderer  │◄─┼──┘
│         └─────┬──────┘    └────────────┘  │   
└───────────────┼───────────────────────────┘   
                │                               
       ┌────────▼──────────┐                    
       │  ┌─────────────┐  │                    
       │  │  std::cout  │  │                    
       │  └─────────────┘  │                    
       └───────────────────┘     
```               

> I tried to use mermaid and plantUML but they're both horrible for setting layout so asciiflow.com clutched up.

## Layer Descriptions

### Base Layer
The fundamental building block of the termui library. Base classes provide core functionality and can be extended to create widgets.

### Widget Layer
Widgets are constructed from Base classes and provide interactive TUI elements. They build upon the foundation provided by the Base layer.

### Interface Layer
Interfaces define the contract for user interaction. Widgets implement these interfaces to provide concrete implementations.

### Display Layer
Differentiated from Base, the Display layer conveys information graphically rather than through plain text. It contains more decorative TUI elements and visual enhancements.

## Terminal and Renderer Design

The Terminal and Renderer architecture is intentionally designed to separate concerns:

- **Terminal**: Aware of its capabilities regarding color and styling support
- **Renderer**: Created alongside the Terminal, it contains the terminal's color capability information
- **Decoupled Design**: Higher-level classes (`Base`, `Widget`, `Interface`) should not contain logic to deal with color and styling details

This separation of concerns allows all classes to pass text and styling information to the Renderer, offloading the complexity of terminal capabilities from the UI layer.
