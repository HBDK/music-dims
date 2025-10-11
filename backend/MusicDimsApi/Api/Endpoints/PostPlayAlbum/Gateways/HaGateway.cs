using System.Text.Json;
using System.Text.Json.Serialization;
using MusicDimsApi.Clients;
using MusicDimsApi.Endpoints.GetArtists.Models;

namespace MusicDimsApi.Endpoints.PostPlayAlbum.Gateways;

public class HaGateway(IHttpClientFactory clientFactory, ILogger<HaGateway> logger) : IHaGateway
{
    private const string GetLibraryRoute = "/api/services/music_assistant/play_media"; //"?return_response=true";

    public async Task PlayAlbum(string id)
    {
        var body = new GetLibraryPostBody("media_player.bathroom_speaker", "album", id);
        var client = clientFactory.CreateClient(HomeAssistantClient.Name);
        var response = await client.PostAsync(GetLibraryRoute, new StringContent(JsonSerializer.Serialize(body), System.Text.Encoding.UTF8, "application/json"));

        var test = await response.Content.ReadAsStringAsync();
        response.EnsureSuccessStatusCode();
    }
}

public record GetLibraryPostBody([property: JsonPropertyName("entity_id")]string EntityId, [property: JsonPropertyName("media_type")]string MediaType, [property: JsonPropertyName("media_id")] string MediaId);
public interface IHaGateway
{
    Task PlayAlbum(string id);
}