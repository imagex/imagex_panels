## Panels & Panelizer

This component enables default configuration for Panels, Mini Panels and Panelizer.

### Requirements

The following listed modules and libraries below are required dependencies of this component.

#### Modules

* Contrib: [Ctools](https://drupal.org/project/ctools)
* Contrib: [Strongarm](https://drupal.org/project/strongarm)

### Variables

- default available layouts for Panel pages and Mini Panels. Both use only layouts available through the Parrot theme.
- Node_view and user_view Panel pages enabled. Node_view in particular is required by all features implementing Panelizer for their content types. In particular when using 'Full Page Override', which is the recommended approach.

### Extension Modules

Every feature including Panelizer settings should depend on this module, especially when using 'Full Page Override'.


## License

This module has been engineered by [ImageX](http://www.imagexmedia.com) and has been licensed under the [GNU General Public License](http://www.gnu.org/licenses/gpl-2.0.html) version 2.
