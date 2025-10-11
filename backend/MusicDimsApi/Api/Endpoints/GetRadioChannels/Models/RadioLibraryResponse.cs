using System.Text.Json.Serialization;

namespace MusicDimsApi.Endpoints.GetRadioChannels.Models;

public record RadioLibraryResponse(
    [property: JsonPropertyName("changed_states")]
    IReadOnlyList<object> ChangedStates,
    [property: JsonPropertyName("service_response")]
    RadioLibraryResponse.ServiceResponseDto ServiceResponse)
{
    public record ServiceResponseDto(
        [property: JsonPropertyName("items")]
        IReadOnlyList<RadioItem> Items,
        [property: JsonPropertyName("limit")]
        int Limit,
        [property: JsonPropertyName("offset")]
        int Offset,
        [property: JsonPropertyName("order_by")]
        string OrderBy,
        [property: JsonPropertyName("media_type")]
        string MediaType);

    public record RadioItem(
        [property: JsonPropertyName("media_type")]
        string MediaType,
        [property: JsonPropertyName("uri")]
        string Uri,
        [property: JsonPropertyName("name")]
        string Name,
        [property: JsonPropertyName("version")]
        string Version,
        [property: JsonPropertyName("image")]
        string? Image);
}