using System.Text.Json.Serialization;

namespace MusicDimsApi.Endpoints.GetArtists.Models;

public record MusicAssistantGetLibraryResponse(
    [property: JsonPropertyName("changed_states")]
    IReadOnlyList<object> ChangedStates,
    [property: JsonPropertyName("service_response")]
    MusicAssistantGetLibraryResponse.ServiceResponseResponse ServiceResponse)
{
    public record ServiceResponseResponse(
        [property: JsonPropertyName("items")]
        IReadOnlyList<ServiceResponseResponse.ItemResponse> Items,
        [property: JsonPropertyName("limit")]
        int Limit,
        [property: JsonPropertyName("offset")]
        int Offset,
        [property: JsonPropertyName("order_by")]
        string OrderBy,
        [property: JsonPropertyName("media_type")]
        string MediaType)
    {
        public record ItemResponse(
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
}