#include "pch.h"

#include "StyleResources.h"
#if __has_include("StyleResources.g.cpp")
#include "StyleResources.g.cpp"
#endif

namespace winrt::TranslucentTB::Xaml::implementation
{
	wux::DependencyProperty StyleResources::m_ResourcesProperty =
		wux::DependencyProperty::RegisterAttached(
			L"Resources",
			winrt::xaml_typename<wux::ResourceDictionary>(),
			winrt::xaml_typename<winrt::TranslucentTB::Xaml::StyleResources>(),
			wux::PropertyMetadata(nullptr, OnResourcesChanged));

	void StyleResources::OnResourcesChanged(const wux::DependencyObject &d, const wux::DependencyPropertyChangedEventArgs &e)
	{
		if (const auto source = d.try_as<wux::FrameworkElement>())
		{
			CloneResourceDictionary(e.NewValue().as<wux::ResourceDictionary>(), source.Resources());
		}
	}

	wux::ResourceDictionary StyleResources::CloneResourceDictionary(const wux::ResourceDictionary &resource, const wux::ResourceDictionary &destination)
	{
		if (!resource)
		{
			return nullptr;
		}

		if (const auto source = resource.Source())
		{
			destination.Source(source);
		}
		else
		{
			if (const auto themeDicts = resource.ThemeDictionaries())
			{
				for (const auto theme : themeDicts)
				{
					if (const auto themeResource = theme.Value().try_as<wux::ResourceDictionary>())
					{
						wux::ResourceDictionary themeDict;
						CloneResourceDictionary(themeResource, themeDict);
						destination.ThemeDictionaries().Insert(theme.Key(), themeDict);
					}
					else
					{
						destination.ThemeDictionaries().Insert(theme.Key(), theme.Value());
					}
				}
			}

			if (const auto mergeDicts = resource.MergedDictionaries())
			{
				for (const auto mergedResource : mergeDicts)
				{
					wux::ResourceDictionary mergedDict;
					CloneResourceDictionary(mergedResource, mergedDict);
					destination.MergedDictionaries().Append(mergedDict);
				}
			}

			for (const auto item : resource)
			{
				destination.Insert(item.Key(), item.Value());
			}
		}

		return destination;
	}
}