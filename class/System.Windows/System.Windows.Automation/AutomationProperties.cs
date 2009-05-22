/*
 * AutomationProperties.cs.
 *
 * Contact:
 *   Moonlight List (moonlight-list@lists.ximian.com)
 *
 * Copyright 2008 Novell, Inc. (http://www.novell.com)
 *
 * See the LICENSE file included with the distribution for details.
 * 
 */

using System;

namespace System.Windows.Automation
{	
	public static class AutomationProperties
	{
		public static readonly DependencyProperty AcceleratorKeyProperty 
			= DependencyProperty.RegisterAttachedCore ("AcceleratorKey", typeof (string), typeof (AutomationProperties), null);
		public static readonly DependencyProperty AccessKeyProperty
			= DependencyProperty.RegisterAttachedCore ("AccessKey", typeof (string), typeof (AutomationProperties), null);
		public static readonly DependencyProperty AutomationIdProperty
			= DependencyProperty.RegisterAttachedCore ("AutomationId", typeof (string), typeof (AutomationProperties), null);
		public static readonly DependencyProperty HelpTextProperty
			= DependencyProperty.RegisterAttachedCore ("HelpText", typeof (string), typeof (AutomationProperties), null);
		public static readonly DependencyProperty IsRequiredForFormProperty
			= DependencyProperty.RegisterAttachedCore ("IsRequiredForForm", typeof (bool), typeof (AutomationProperties), new PropertyMetadata (false));
		public static readonly DependencyProperty ItemStatusProperty
			= DependencyProperty.RegisterAttachedCore ("ItemStatus", typeof (string), typeof (AutomationProperties), null);
		public static readonly DependencyProperty ItemTypeProperty
			= DependencyProperty.RegisterAttachedCore ("ItemType", typeof (string), typeof (AutomationProperties), null);
		public static readonly DependencyProperty LabeledByProperty
			= DependencyProperty.RegisterAttachedCore ("LabeledBy", typeof (UIElement), typeof (AutomationProperties), null);
		public static readonly DependencyProperty NameProperty
			= DependencyProperty.RegisterAttachedCore ("Name", typeof (string), typeof (AutomationProperties), null);
		
		public static string GetAcceleratorKey(DependencyObject element)
		{
			return (string) element.GetValue (AutomationProperties.AcceleratorKeyProperty);
		}
		
		public static string GetAccessKey(DependencyObject element)
		{
			return (string) element.GetValue (AutomationProperties.AccessKeyProperty);
		}
		
		public static string GetAutomationId(DependencyObject element)
		{
			return (string) element.GetValue (AutomationProperties.AutomationIdProperty);
		}
		
		public static string GetHelpText(DependencyObject element)
		{
			return (string) element.GetValue (AutomationProperties.HelpTextProperty);
		}
		
		public static bool GetIsRequiredForForm(DependencyObject element)
		{
			return (bool) element.GetValue (AutomationProperties.IsRequiredForFormProperty);
		}
		
		public static string GetItemStatus(DependencyObject element)
		{
			return (string) element.GetValue (AutomationProperties.ItemStatusProperty);
		}
		
		public static string GetItemType(DependencyObject element)
		{
			return (string) element.GetValue (AutomationProperties.ItemTypeProperty);
		}
		
		public static UIElement GetLabeledBy(DependencyObject element)
		{
			return (UIElement) element.GetValue (AutomationProperties.LabeledByProperty);
		}
		
		public static string GetName(DependencyObject element)
		{
			return (string) element.GetValue (AutomationProperties.NameProperty);
		}
		
		public static void SetAcceleratorKey(DependencyObject element, string value)
		{
			element.SetValue (AutomationProperties.AcceleratorKeyProperty, value);
		}
		
		public static void SetAccessKey(DependencyObject element, string value)
		{
			element.SetValue (AutomationProperties.AccessKeyProperty, value);
		}
		
		public static void SetAutomationId(DependencyObject element, string value)
		{
			element.SetValue (AutomationProperties.AutomationIdProperty, value);
		}
		
		public static void SetHelpText(DependencyObject element, string value)
		{
			element.SetValue (AutomationProperties.HelpTextProperty, value);
		}
		
		public static void SetIsRequiredForForm(DependencyObject element, bool value)
		{
			element.SetValue (AutomationProperties.IsRequiredForFormProperty, value);
		}
		
		public static void SetItemStatus(DependencyObject element, string value)
		{
			element.SetValue (AutomationProperties.ItemStatusProperty, value);
		}
		
		public static void SetItemType(DependencyObject element, string value)
		{
			element.SetValue (AutomationProperties.ItemTypeProperty, value);
		}
		
		public static void SetLabeledBy(DependencyObject element, UIElement value)
		{
			element.SetValue (AutomationProperties.LabeledByProperty, value);
		}
		
		public static void SetName(DependencyObject element, string value)
		{
			element.SetValue (AutomationProperties.NameProperty, value);
		}
		

	}
}
